#pragma once

#ifndef STDAFX_H_
#define STDAFX_H_

#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <vector>
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

// �� ����
extern constexpr char ACTION_NONE = 100; // ���� ����
extern constexpr char ACTION_MOVETOPLAYER = 101; // �÷��̾ ���� �̵�
extern constexpr char ACTION_ATTACKPLAYER = 102; // �÷��̾� ����

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

/* ----- ������� �߰��� �κ� ----- */

// ������Ʈ ����ü

extern struct Object
{
	float scale_x{ 1.0f }, scale_y{ 1.0f }, scale_z{ 1.0f };    // ũ�� ����
	float rotate_x{ 0.0f }, rotate_y{ 0.0f }, rotate_z{ 0.0f }; // ȸ�� ����
	float pos_x{ 0.0f }, pos_y{ 0.0f }, pos_z{ 0.0f };          // ��ġ

	// '�÷��̾�' ������Ʈ��
	bool bNowMoving{ false }; // ���� �̵��� ����

	// '��' ������Ʈ��
	char cNowAction{ ACTION_NONE }; // ���� ����
};

// �� �κ��� ���� ���� ��ġ�� d3dApp.cpp�̴�.
extern int g_id; // �÷��̾� ID
extern Object floorobj;  // �ٴ�
extern Object player[2]; // �÷��̾� 2��
extern Object enemy_s;   // ���� ��
extern Object enemy_m;   // ���� ��
extern Object enemy_l;   // ���� ��

#endif 

