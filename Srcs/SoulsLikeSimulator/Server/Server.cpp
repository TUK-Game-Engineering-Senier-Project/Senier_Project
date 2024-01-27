// ----- 서버 cpp 파일 -----

#include "Stdafx.h" // 사용자 정의 헤더 파일
#include "ServerFunctions.h" // 서버 기능 헤더 파일
#include "GameObject.h" // 게임 오브젝트 헤더 파일

using namespace std;

mutex g_mClientListMutex; // 클라이언트 리스트용 뮤텍스
vector<g_clientInfo> g_vClientList; // 클라이언트 목록
vector<g_cPlayer*> g_vPlayers; // 플레이어
SOCKET g_sListenSock; // 수신용 소켓

// 랜덤 디바이스
random_device rd;
mt19937 gen{ rd() };
// uniform_int_distribution<int> uid{ 0,2000 };

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

// ----- main 함수 -----

vector<g_clientInfo> ClientList;
vector<g_cPlayer*> g_Players;

int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	g_sListenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_sListenSock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(g_sListenSock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(g_sListenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

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
		// accept()
		iAddrlen = sizeof(saClientAddr);
		sClientSock = accept(g_sListenSock, (struct sockaddr*)&saClientAddr, &iAddrlen);
		if (g_sListenSock == 0)break;
		if (sClientSock == INVALID_SOCKET) {
			err_quit("accept()");
			break;
		}
		lock_guard<mutex> lock(g_mClientListMutex);

		siClients[iPlayerNum - 1].sock = sClientSock;
		siClients[iPlayerNum - 1].id = (iPlayerNum - 1);
		g_cPlayer* newPlayer = new g_cPlayer;
		g_Players.push_back(newPlayer);

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
	//closesocket(g_sListenSock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
