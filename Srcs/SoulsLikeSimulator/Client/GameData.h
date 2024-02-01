#pragma once

// ���� ���� ã�ƺ��ϱ� winsock.h(�� ���Ե�) �����
// Windows.h ���� �޾���� �Ѵٰ� �Ͽ� ��ġ ������
#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���
#include <WindowsX.h>

// �÷��̾� ����ü
struct Player
{
	float trans_x{}, trans_y{}, trans_z{};
	float rotate_x{}, rotate_y{}, rotate_z{};
	float scale_x{ 1.0f }, scale_y{ 1.0f }, scale_z{ 1.0f };
	// float color_r{}, color_g{}, color_b{};
};

// �÷��̾� ��ü 2�� (2��)
Player player[2];

// Ŭ���̾�Ʈ ����
struct g_clientInfo
{
	SOCKET sock; // ����
	int id; // ���� ID
};

// ���� ����
struct g_sockInfo
{
	SOCKET sock; // ����
	int id; // ���� ID

	// ���� ���� ���� �Լ�
	g_sockInfo* GetSockInfo() { return this; }
};

int g_iId;
g_sockInfo* siSockInfo;
SOCKET sock;

constexpr char SC_PLAYER_MOVE = 0;
constexpr char SC_KEY_INPUT = 1;
constexpr char SC_PLAYER_ROTATE = 2;
constexpr char SC_SEND_PLAYER = 3;

// �Է� ��Ŷ
struct INPUT_PACKET
{
	char type;
	bool bKeyDown;
	char input;
};

// �̵� ��Ŷ
struct MOVE_PACKET
{
	char type;
	float fx, fy, fz;
	float rot;
};

// ȸ�� ��Ŷ
struct ROTATE_PACKET
{
	char type;
	float fx, fy, fz;
};

// �÷��̾� ����
struct SEND_PLAYER {
	char type;
	int id;
};



// ���� �Լ� ���� ��� �� ����
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

// ���� ��� (����, ����)
void info_show(const char* title, const char* text)
{
	HWND hwnd = NULL;
	LPCSTR lpText = text;
	LPCSTR lpCaption = title;
	UINT uType = MB_OK | MB_ICONINFORMATION;

	MessageBoxA(hwnd, lpText, lpCaption, uType);
}

