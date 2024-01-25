#pragma once

#ifndef STDAFX_H_
#define STDAFX_H_

#include <mutex> // ���� ������ ���ؽ� ��� 
#include <vector> // ���� ����Ʈ ������
#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include "../Common/d3dApp.h"
#include "../Common/MathHelper.h"
#include "../Common/UploadBuffer.h"

#include "Objects.h" // ������Ʈ ���� ���� ���

#define SERVERPORT 9000 // ���� ��Ʈ ��ȣ
#define BUFSIZE    512  // ���� ũ��

constexpr char SC_KEY_INPUT = 1; // Ű �Է½�

struct INPUT_PACKET {
	char type;
	bool bKeyDown;
	char input;
};

#endif 