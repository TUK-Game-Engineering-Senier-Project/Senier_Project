// ----- 서버 cpp 파일 -----

#include "Stdafx.h" // 사용자 정의 헤더 파일

using namespace std;

mutex g_mClientListMutex; // 클라이언트 리스트용 뮤텍스
vector<g_clientInfo> g_vClientList; // 클라이언트 목록
vector<g_cPlayer*> g_vPlayers; // 플레이어
SOCKET g_sListenSock; // 수신용 소켓

// 랜덤 디바이스
random_device rd;
mt19937 gen{ rd() };
// uniform_int_distribution<int> uid{ 0,2000 };

// 각 상황에 따른 값
constexpr char SC_KEY_INPUT = 1; // 키 입력시

// 입력 패킷
struct INPUT_PACKET {
	char type;
	bool bKeyDown;
	char input;
};

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ----- 키 입력 정보 전송 함수 -----

void SendKeyInput(g_sockInfo* sockInfo, char cInput, bool bKeyDown)
{
	// 키 입력별 정보 전송
	switch (cInput) {
	case VK_UP:
	{
		g_vPlayers[sockInfo->id]->bForwardKeyDown = bKeyDown;
		break;
	}
	case VK_LEFT:
	{
		g_vPlayers[sockInfo->id]->bLeftKeyDown = bKeyDown;
		break;
	}
	case VK_DOWN:
	{
		g_vPlayers[sockInfo->id]->bBackKeyDown = bKeyDown;
		break;
	}
	case VK_RIGHT:
	{
		g_vPlayers[sockInfo->id]->bRightKeyDown = bKeyDown;
		break;
	}
	}
}

// ----- 클라이언트와 데이터 통신 함수 -----

DWORD WINAPI ProcessClient(LPVOID arg)
{
	int iRetval;
	g_sockInfo* sockInfo = reinterpret_cast<g_sockInfo*> (arg);

	// 클라이언트 소켓 통신용 변수
	SOCKET sClientSock = sockInfo->sock;
	struct sockaddr_in siClientAddr;
	char cAddr[INET_ADDRSTRLEN];
	int iAddrlen;
	char cBuf[BUFSIZE + 1];

	// 클라이언트 정보 얻기
	iAddrlen = sizeof(siClientAddr);
	getpeername(sClientSock, (struct sockaddr*)&siClientAddr, &iAddrlen);
	inet_ntop(AF_INET, &siClientAddr.sin_addr, cAddr, sizeof(cAddr));

	while (1) {
		// 업데이트 간격
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));

		// 데이터 보내기

		// 데이터 받기
		iRetval = recv(sClientSock, cBuf, BUFSIZE, 0);
		if (iRetval == SOCKET_ERROR) {
			err_quit("Error: ProcessClient() 내 소켓 recv()");
			break;
		}
		switch (cBuf[0]) {
			// 키 입력일 경우
			case SC_KEY_INPUT:
			{
				// 입력 패킷
				INPUT_PACKET* packet = reinterpret_cast<INPUT_PACKET*>(cBuf);
				//cout << "[" << sock_info->id << "]" << packet->input << endl;
				SendKeyInput(sockInfo, packet->input, packet->bKeyDown);
				break;
			}
		}
	}

	// 소켓 닫기
	closesocket(sClientSock);

	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		cAddr, ntohs(siClientAddr.sin_port));
	return 0;
}

// ----- 플레이어 정보 업데이트 함수 -----

void UpdatePlayer(vector<g_cPlayer*> vcp) {
	bool flag = false;
	bool pFlag = false;
	for (int id = 0; id < 2; id++) {
		if (vcp[id]->bForwardKeyDown) {
			vcp[id]->fPos[0] -= 0.03f * sin(vcp[id]->fRotate[1] * atan(1) * 4 / 180);
			vcp[id]->fPos[2] -= 0.03f * cos(vcp[id]->fRotate[1] * atan(1) * 4 / 180);
		}
		if (vcp[id]->bBackKeyDown) {
			vcp[id]->fPos[0] += 0.03f * sin(vcp[id]->fRotate[1] * atan(1) * 4 / 180);
			vcp[id]->fPos[2] += 0.03f * cos(vcp[id]->fRotate[1] * atan(1) * 4 / 180);
		}
		if (vcp[id]->bLeftKeyDown) {
			vcp[id]->fRotate[1] += 1.f;
		}
		if (vcp[id]->bRightKeyDown) {
			vcp[id]->fRotate[1] -= 1.f;
		}
	}
}

// ----- main 함수 -----

int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	g_sListenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_sListenSock == INVALID_SOCKET) err_quit("Error: 소켓 생성");

	// 소켓 bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(g_sListenSock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("Error: 소켓 bind()");

	// 소켓 listen()
	retval = listen(g_sListenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("Error: 소켓 listen()");

	// 데이터 통신에 사용할 변수
	g_sockInfo* siClients = new g_sockInfo[2]; // 클라이언트 (2인)
	SOCKET sClientSock; // 클라이언트 소켓
	struct sockaddr_in saClientAddr; // 클라이언트 주소
	HANDLE hThread, hSendThread; // 스레드 핸들
	int iAddrlen; // 주소 길이 
	int iPlayerNum = 1; // 현재 플레이어 번호
	
	// 서버 실행 시작 알림
	printf("<서버 실행 시작>\n\n");

	while (1) {
		// 소켓 accept()
		iAddrlen = sizeof(saClientAddr);
		sClientSock = accept(g_sListenSock, (struct sockaddr*)&saClientAddr, &iAddrlen);
		if (sClientSock == 0) break;
		if (sClientSock == INVALID_SOCKET) {
			err_quit("Error: 소켓 accept()");
			break;
		}

		// 뮤텍스 잠그기
		lock_guard<mutex> lock(g_mClientListMutex);

		// 현재 번호에 맞는 플레이어 지정
		siClients[iPlayerNum - 1].sock = sClientSock;
		siClients[iPlayerNum - 1].id = (iPlayerNum - 1);
		g_cPlayer* newPlayer = new g_cPlayer;
		g_vPlayers.push_back(newPlayer);

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &saClientAddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(saClientAddr.sin_port));

		// 스레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient,
			reinterpret_cast<LPVOID*>(siClients[iPlayerNum].GetSockInfo()), 0, NULL);
		if (hThread == NULL) { closesocket(sClientSock); }
		else { CloseHandle(hThread); }
	}

	// 소켓 닫기
	closesocket(sClientSock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
