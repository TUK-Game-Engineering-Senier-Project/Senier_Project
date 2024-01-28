#pragma once

#ifndef STDAFX_H_
#define STDAFX_H_

#include <iostream>
#include <random>
#include <thread>
#include <math.h>

#include <mutex> // ���� ������ ���ؽ� ��� 
#include <vector> // ���� ����Ʈ ������
#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include "../Common/d3dApp.h"
#include "../Common/MathHelper.h"
#include "../Common/UploadBuffer.h"

#include "Objects.h" // ������Ʈ ���� ���� ���

char* SERVERIP = (char*)"127.0.0.1"; // ���� IP �ּ�
#define SERVERPORT 9000 // ���� ��Ʈ ��ȣ
#define BUFSIZE    512  // ���� ũ��

#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include <tchar.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

constexpr char SC_KEY_INPUT = 1; // Ű �Է½�

struct INPUT_PACKET {
	char type;
	bool bKeyDown;
	char input;
};

#endif 

using namespace std;

// Ŭ���̾�Ʈ ����
struct g_clientInfo {
	SOCKET sock; // ����
	int id; // ���� ID
};

// ���� ����
struct g_sockInfo {
	SOCKET sock; // ����
	int id; // ���� ID

	// ���� ���� ���� �Լ�
	g_sockInfo* GetSockInfo() { return this; }
};

random_device rd;
mt19937 gen{ rd() };

int g_iId;
g_sockInfo* siSockInfo;
SOCKET sock;