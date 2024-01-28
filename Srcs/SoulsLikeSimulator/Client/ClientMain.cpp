// ----- 클라이언트 cpp 파일 -----

#include "Stdafx.h" // 사용자 정의 헤더

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

/*
// 패킷에 키보드 누른 입력값을 전송
void KeyBoardDown(unsigned char key, int x, int y)
{
	INPUT_PACKET* packet = new INPUT_PACKET;
	switch (key)
	{
	case VK_LEFT:
		packet->type = SC_KEY_INPUT;
		packet->input = VK_LEFT;
		packet->bKeyDown = true;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	case VK_RIGHT:
		packet->type = SC_KEY_INPUT;
		packet->input = VK_RIGHT;
		packet->bKeyDown = true;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	case VK_UP:
		packet->type = SC_KEY_INPUT;
		packet->input = VK_UP;
		packet->bKeyDown = true;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	case VK_DOWN:
		packet->type = SC_KEY_INPUT;
		packet->input = VK_DOWN;
		packet->bKeyDown = true;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	}
	delete packet;
}

// 패킷에 키보드 뗀 입력값을 전송
void KeyBoardUp(unsigned char key, int x, int y)
{
	INPUT_PACKET* packet = new INPUT_PACKET;
	switch (key)
	{
	case VK_LEFT:
		packet->type = SC_KEY_INPUT;
		packet->input = VK_LEFT;
		packet->bKeyDown = false;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	case VK_RIGHT:
		packet->type = SC_KEY_INPUT;
		packet->input = VK_RIGHT;
		packet->bKeyDown = false;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	case VK_UP:
		packet->type = SC_KEY_INPUT;
		packet->input = VK_UP;
		packet->bKeyDown = false;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	case VK_DOWN:
		packet->type = SC_KEY_INPUT;
		packet->input = VK_DOWN;
		packet->bKeyDown = false;
		send(sock, reinterpret_cast<char*>(packet), sizeof(INPUT_PACKET), 0);
		break;
	}
	delete packet;
}
*/

// ----- WinMain -----

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
    PSTR cmdLine, int showCmd)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    try
    {
        BoxApp theApp(hInstance);
        if (!theApp.Initialize())
            return 0;

		// 네트워크 설정
		int iRetval;

		// 윈속 초기화
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			return 1;

		// 소켓 생성
		siSockInfo = new g_sockInfo;

		sock = socket(AF_INET, SOCK_STREAM, 0);

		if (sock == INVALID_SOCKET) err_quit("Error: 소켓 생성");

		// connect()
		struct sockaddr_in serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
		serveraddr.sin_port = htons(SERVERPORT);
		iRetval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (iRetval == SOCKET_ERROR) err_quit("Error: 소켓 connect()");
		siSockInfo->sock = sock;

		closesocket(sock);

		// 윈속 종료
		WSACleanup();
		return 0;

        // 프로그램 실행
        return theApp.Run();
    }
    catch (DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }
}