#pragma once

// <����� ���� ��� ����>

#ifndef STDAFX_H_ // ������ ���� if�� ����
#define STDAFX_H_

constexpr int SERVERPORT = 9000;     // ���� ��Ʈ ��ȣ
constexpr int BUFSIZE = 512;         // ���� ũ��

#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mutex> // ���ؽ�
#include <vector> // ����
#include <random> // ������

// #include <string>
// #include <stdio.h>
// #include <iostream>
// #include <chrono>

// #include "..\Protocol.h"
#include "GameObject.h" // ���� ������Ʈ ���

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

// ----- ����� ���� ��, Ŭ���� -----

extern float g_fPlayerSpd;  // �̵� �ӵ�
extern float g_fJumpSpd; // ���� �ӵ�


// Ŭ���̾�Ʈ ����
struct ClientInfo {
	SOCKET socket;
	int id;
};

// ���� ����
struct SOCK_INFO {
	SOCKET client_sock{};
	int id{};

	SOCK_INFO* GetSockInfo() { return this; }
};

constexpr char SC_PLAYER_MOVE = 0;
constexpr char SC_KEY_INPUT = 1;
constexpr char SC_PLAYER_ROTATE = 2;
constexpr char SC_SEND_PLAYER = 3;

// �Է� ��Ŷ
struct INPUT_PACKET {
	char type;
	bool bKeyDown;
	char input;
};

// �̵� ��Ŷ
struct MOVE_PACKET {
	char type;
	float fx, fy, fz;
	float rot;
};

// ȸ�� ��Ŷ
struct ROTATE_PACKET {
	char type;
	float fx, fy, fz;
};

// �÷��̾� ����
struct SEND_PLAYER {
	char type;
	int id;
};

#endif