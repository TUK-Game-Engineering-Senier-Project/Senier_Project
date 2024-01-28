#pragma once

// <����� ���� ��� ����>

#ifndef STDAFX_H_ // ������ ���� if�� ����
#define STDAFX_H_

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

#endif // ������ ���� if�� ����

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
