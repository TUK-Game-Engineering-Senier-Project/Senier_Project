#pragma once

// 오류 나서 찾아보니까 winsock.h(가 포함된) 헤더를
// Windows.h 위에 달아줘야 한다고 하여 위치 변경함
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더
#include <WindowsX.h>

// 플레이어 구조체
struct Player
{
	float trans_x{}, trans_y{}, trans_z{};
	float rotate_x{}, rotate_y{}, rotate_z{};
	float scale_x{ 1.0f }, scale_y{ 1.0f }, scale_z{ 1.0f };
	// float color_r{}, color_g{}, color_b{};
};

// 플레이어 개체 2개 (2인)
Player player[2];

// 클라이언트 정보
struct g_clientInfo
{
	SOCKET sock; // 소켓
	int id; // 소켓 ID
};

// 소켓 정보
struct g_sockInfo
{
	SOCKET sock; // 소켓
	int id; // 소켓 ID

	// 소켓 정보 리턴 함수
	g_sockInfo* GetSockInfo() { return this; }
};

int g_iId;
g_sockInfo* siSockInfo;
SOCKET sock;

constexpr char SC_PLAYER_MOVE = 0;
constexpr char SC_KEY_INPUT = 1;
constexpr char SC_PLAYER_ROTATE = 2;
constexpr char SC_SEND_PLAYER = 3;

// 입력 패킷
struct INPUT_PACKET
{
	char type;
	bool bKeyDown;
	char input;
};

// 이동 패킷
struct MOVE_PACKET
{
	char type;
	float fx, fy, fz;
	float rot;
};

// 회전 패킷
struct ROTATE_PACKET
{
	char type;
	float fx, fy, fz;
};

// 플레이어 전송
struct SEND_PLAYER {
	char type;
	int id;
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

// 정보 출력 (제목, 내용)
void info_show(const char* title, const char* text)
{
	HWND hwnd = NULL;
	LPCSTR lpText = text;
	LPCSTR lpCaption = title;
	UINT uType = MB_OK | MB_ICONINFORMATION;

	MessageBoxA(hwnd, lpText, lpCaption, uType);
}

