#pragma once

#ifndef STDAFX_H_
#define STDAFX_H_

#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <mutex> // ���� ������ ���ؽ� ��� 
// ���� ���� ã�ƺ��ϱ� winsock.h(�� ���Ե�) �����
// Windows.h ���� �޾���� �Ѵٰ� �Ͽ� ��ġ ������
#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

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

extern constexpr char SC_PLAYER_MOVE = 0;
extern constexpr char SC_KEY_INPUT = 1;
extern constexpr char SC_PLAYER_ROTATE = 2;
extern constexpr char SC_SEND_PLAYER = 3;

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
	char type; // ����
	float fPx, fPy, fPz; // ��ġ
	float fRx, fRy, fRz; // ȸ��
	float fMx, fMy, fMz; // �̵�
};;


// ȸ�� ��Ŷ
struct ROTATE_PACKET
{
	char type;
	float fx, fy, fz;
};

// �÷��̾� ����
struct SEND_PLAYER 
{
	char type;
	int id;
};

// �÷��̾� ����ü
extern struct Player
{
	float pos_x{ 0.0f },    pos_y{ 0.0f },    pos_z{ 0.0f };    // ��ġ
	float scale_x{ 0.3f },  scale_y{ 0.3f },  scale_z{ 0.3f };  // ũ�� ����
	float rotate_x{ 0.0f }, rotate_y{ 0.0f }, rotate_z{ 0.0f }; // ȸ�� ����
	float move_x{ 0.0f },   move_y{ 0.0f },   move_z{ 0.0f };   // �̵� �Ÿ�
	// float color_r{}, color_g{}, color_b{};
};

extern int g_id; // �÷��̾� ID
extern Player player[2]; // �÷��̾� 2��
extern Player playerLookDir[2]; // �÷��̾� ���� ���� ����

#endif 

