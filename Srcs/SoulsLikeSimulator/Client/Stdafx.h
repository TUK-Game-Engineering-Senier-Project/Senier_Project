#pragma once

#ifndef STDAFX_H_
#define STDAFX_H_

#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <mutex>      // ���� ������ ���ؽ� ���
#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "ws2_32") 

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

// ���� ����
extern constexpr char SC_UPDATE = 10;
extern constexpr char SC_PLAYER_MOVE = 0;
extern constexpr char SC_KEY_INPUT = 1;
extern constexpr char SC_PLAYER_ROTATE = 2;
extern constexpr char SC_SEND_PLAYER = 3;

// ����
extern constexpr char ACTION_NONE = 100;

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
	char cType; // ����
	float fPx, fPy, fPz; // ��ġ
	float fRx, fRy, fRz; // ȸ��
	float fMx, fMy, fMz; // �̵�

	float fRadius; // �þ� ��ġ

	bool bNowMoving{ false }; // ���� �̵��� ����

	char cNowAction{ ACTION_NONE }; // ���� ����
};


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
	float scale_x{ 0.3f }, scale_y{ 0.3f }, scale_z{ 0.3f };    // ũ�� ����
	float rotate_x{ 0.0f }, rotate_y{ 0.0f }, rotate_z{ 0.0f }; // ȸ�� ����
	float pos_x{ 0.0f },    pos_y{ 0.0f },    pos_z{ 0.0f };    // ��ġ
	float move_x{ 0.0f },   move_y{ 0.0f },   move_z{ 0.0f };   // �̵� �Ÿ�

	float fRadius{ 10.0f }; // �þ� ��ġ

	bool bNowMoving{ false };       // ���� �̵��� ����
	char cNowAction{ ACTION_NONE }; // ���� ����

	// float color_r{}, color_g{}, color_b{};
};

extern int g_id; // �÷��̾� ID
extern Player player[2]; // �÷��̾� 2��

#endif 

