#include "Stdafx.h" // 사용자 정의 헤더 파일

using namespace std;

// 데이터 통신에 사용할 변수
mutex clientListMutex; // 뮤텍스
vector<ClientInfo> ClientList; // 클라이언트 목록
vector<g_cPlayer*> g_Players; // 플레이어 클래스들
SOCKET listen_sock{}; // listen 소켓

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

// 플레이어 이동 전달 함수
void SendPlayerMove(SOCK_INFO* sock_info, char input, bool KeyDown) 
{
	switch (input) 
	{
		// 화살표 키 (이동)
		case VK_LEFT: {g_Players[sock_info->id]->bLeftKeyDown = KeyDown; printf("왼쪽 방향키 입력\n");
			// ### 테스트용
			printf("### 테스트용 : (%.2f, %.2f, %.2f)\n",
				g_Players[sock_info->id]->fPos[0], g_Players[sock_info->id]->fPos[1], g_Players[sock_info->id]->fPos[2]);
			break; }

		case VK_RIGHT: {g_Players[sock_info->id]->bRightKeyDown = KeyDown; printf("오른쪽 방향키 입력\n");
			// ### 테스트용
			printf("### 테스트용 : (%.2f, %.2f, %.2f)\n",
				g_Players[sock_info->id]->fPos[0], g_Players[sock_info->id]->fPos[1], g_Players[sock_info->id]->fPos[2]);
			break; }

		case VK_UP: {g_Players[sock_info->id]->bForwardKeyDown = KeyDown; printf("위 방향키 입력\n");
			// ### 테스트용
			printf("### 테스트용 : (%.2f, %.2f, %.2f)\n",
				g_Players[sock_info->id]->fPos[0], g_Players[sock_info->id]->fPos[1], g_Players[sock_info->id]->fPos[2]);
			break; }

		case VK_DOWN: {g_Players[sock_info->id]->bBackKeyDown = KeyDown; printf("아래 방향키 입력\n");
			// ### 테스트용
			printf("### 테스트용 : (%.2f, %.2f, %.2f)\n",
				g_Players[sock_info->id]->fPos[0], g_Players[sock_info->id]->fPos[1], g_Players[sock_info->id]->fPos[2]);
			break; }

		// 스페이스바 (점프)
		case VK_SPACE: {g_Players[sock_info->id]->bBackKeyDown = KeyDown; printf("점프 키 (스페이스바) 입력\n"); 
			// ### 테스트용
			printf("### 테스트용 : (%.2f, %.2f, %.2f)\n",
				g_Players[sock_info->id]->fPos[0], g_Players[sock_info->id]->fPos[1], g_Players[sock_info->id]->fPos[2]);
			break; }
	}
}

// 플레이어 업데이트
void UpdatePlayer() 
{
	// 각 플레이어에 대하여
	for (int id = 0; id < 1; id++) { // ### 테스트용 - 나중에 id < 2로 수정 예정

		// 입력된 키에 따라 동작 수행하기

		if (g_Players[id]->bForwardKeyDown) {
			g_Players[id]->fPos[0] -= 0.03f * sin(g_Players[id]->fRotate[1] * atan(1) * 4 / 180);
			g_Players[id]->fPos[2] -= 0.03f * cos(g_Players[id]->fRotate[1] * atan(1) * 4 / 180);
		}
		if (g_Players[id]->bBackKeyDown) {
			g_Players[id]->fPos[0] += 0.03f * sin(g_Players[id]->fRotate[1] * atan(1) * 4 / 180);
			g_Players[id]->fPos[2] += 0.03f * cos(g_Players[id]->fRotate[1] * atan(1) * 4 / 180);
		}
		if (g_Players[id]->bLeftKeyDown) {
			g_Players[id]->fRotate[1] += 1.f;
		}
		if (g_Players[id]->bRightKeyDown) {
			g_Players[id]->fRotate[1] -= 1.f;
		}
	}
}

// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	// 데이터 통신에 사용할 변수
	int iRetval;
	SOCK_INFO* sock_info = reinterpret_cast<SOCK_INFO*> (arg);
	SOCKET client_sock = sock_info->client_sock;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	// ----- 클라이언트 Process 루프 시작 ----- //
	while (1) 
	{
		// 플레이어 정보 업데이트
		UpdatePlayer(); 

		// 각 클라이언트의 데이터 보내기
		for (const auto& client : ClientList) {

			// 패킷 생성
			SEND_PLAYER* packet_sendP = new SEND_PLAYER;
			packet_sendP->type = SC_SEND_PLAYER;
			packet_sendP->id = sock_info->id;

			// 패킷에 값 지정
			MOVE_PACKET* packet_tr = new MOVE_PACKET;
			packet_tr->type = SC_PLAYER_MOVE;
			packet_tr->fx = g_Players[sock_info->id]->fPos[0];
			packet_tr->fy = g_Players[sock_info->id]->fPos[1];
			packet_tr->fz = g_Players[sock_info->id]->fPos[2];
			packet_tr->rot = g_Players[sock_info->id]->fRotate[1];

			// 패킷 보내기
			send(client.socket, reinterpret_cast<char*>(packet_sendP), sizeof(SEND_PLAYER), 0);
			send(client.socket, reinterpret_cast<char*>(packet_tr), sizeof(MOVE_PACKET), 0);

			// 패킷 지우기
			delete packet_sendP;
			delete packet_tr;

			printf("### 클라이언트 Process - 데이터 전송 완료\n");
		}

		// 데이터 받기
		iRetval = recv(client_sock, buf, BUFSIZE, 0);
		if (iRetval == SOCKET_ERROR) {
			err_quit("recv()");
			break;
		}
		
		// 버퍼 값에 따라 동작 수행
		switch (buf[0]) {
			// 키 입력받는 경우
			case SC_KEY_INPUT:
			{
				// 플레이어 이동 전달하기
				INPUT_PACKET* packet = reinterpret_cast<INPUT_PACKET*>(buf);
				SendPlayerMove(sock_info, packet->input, packet->bKeyDown);
				break;
			}
		}
	}
	// ----- 클라이언트 Process 루프 종료 ----- //

	// 소켓 닫기
	closesocket(client_sock);
	return 0;
}

int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("Error: 소켓 생성");

	// 소켓 bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("Error: 소켓 bind()");

	// 소켓 listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("Error: 소켓 listen()");

	// 데이터 통신에 사용할 변수
	SOCK_INFO* Clients = new SOCK_INFO[2]; // 플레이어 2명 (1 대 1 PVP)
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	HANDLE hThread, hSendThread;
	int addrlen;
	int iPlayerNum = 0; // 플레이어 번호(순서)

	// 서버 열기 성공 알리기
	printf("<서버 열기 성공!>\n\n");

	// ----- 서버 루프 시작 ----- //
	while (1) {
		// 소켓 accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (listen_sock == 0)break;
		if (client_sock == INVALID_SOCKET) {
			err_quit("Error: 소켓 accept()");
			break;
		}

		// 클라이언트 뮤텍스 잠그기 (동시접근 방지)
		lock_guard<mutex> lock(clientListMutex);

		// 플레이어 id 지정
		Clients[iPlayerNum].client_sock = client_sock;
		Clients[iPlayerNum].id = iPlayerNum;

		// 클라이언트 추가
		ClientInfo newClient;
		newClient.socket = Clients->client_sock;
		newClient.id = Clients->id;
		ClientList.push_back(newClient);

		// 플레이어 추가
		g_cPlayer* newPlayer = new g_cPlayer;
		g_Players.push_back(newPlayer);

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));

		// 스레드 생성 -> ProcessClient()에서 처리
		hThread = CreateThread(NULL, 0, ProcessClient,
			reinterpret_cast<LPVOID*>(Clients[iPlayerNum].GetSockInfo()), 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }

		iPlayerNum++; // 다음 플레이어로
	}
	// ----- 서버 루프 종료 ----- //

	// 소켓 닫기
	closesocket(listen_sock);
	// 윈속 종료
	WSACleanup();

	printf("<서버 닫기 완료>\n");
	return 0;
}