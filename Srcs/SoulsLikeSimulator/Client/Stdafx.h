#pragma once

#ifndef STDAFX_H_
#define STDAFX_H_

#include <iostream>
#include <random>
#include <thread>
#include <math.h>

#include <mutex> // ���� ������ ���ؽ� ��� 
#include <vector> // ���� ����Ʈ ������
// ���� ���� ã�ƺ��ϱ� winsock.h(�� ���Ե�) �����
// Windows.h ���� �޾���� �Ѵٰ� �Ͽ� ��ġ ������
#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���
#include <WindowsX.h>

#include "Objects.h" // ������Ʈ ���� ���� ���

#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

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
extern struct INPUT_PACKET
{
	char type;
	bool bKeyDown;
	char input;
};

// �̵� ��Ŷ
extern struct MOVE_PACKET
{
	char type; // ����
	float fx, fy, fz; // ��ġ
	// float rot;
};

// ȸ�� ��Ŷ
extern struct ROTATE_PACKET
{
	char type;
	float fx, fy, fz;
};

// �÷��̾� ����
extern struct SEND_PLAYER {
	char type;
	int id;
};

// �÷��̾� ����ü
extern struct Player
{
	float trans_x{}, trans_y{}, trans_z{};
	float scale_x{ 0.5f }, scale_y{ 1.0f }, scale_z{ 0.5f };
	// float rotate_x{}, rotate_y{}, rotate_z{};
	// float color_r{}, color_g{}, color_b{};
};

// �÷��̾� ����ü
extern struct PlayerLookDir
{
	float trans_x{}, trans_y{}, trans_z{};
	float scale_x{ 0.1f }, scale_y{ 0.1 }, scale_z{ 0.1f };
	// float rotate_x{}, rotate_y{}, rotate_z{};
	// float color_r{}, color_g{}, color_b{};
};

extern int g_id; // �÷��̾� ID
extern Player player[2]; // �÷��̾� 2��
extern PlayerLookDir playerLookDir[2]; // �÷��̾� ���� ���� ����

#endif 

