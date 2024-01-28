#pragma once

#ifndef STDAFX_H_
#define STDAFX_H_

#include <iostream>
#include <random>
#include <thread>
#include <math.h>

#include <mutex> // 서버 구성용 뮤텍스 헤더 
#include <vector> // 벡터 리스트 생성용
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include "../Common/d3dApp.h"
#include "../Common/MathHelper.h"
#include "../Common/UploadBuffer.h"

#include "Objects.h" // 오브젝트 관련 내용 헤더

char* SERVERIP = (char*)"127.0.0.1"; // 서버 IP 주소
#define SERVERPORT 9000 // 서버 포트 번호
#define BUFSIZE    512  // 버퍼 크기

#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include <tchar.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

constexpr char SC_KEY_INPUT = 1; // 키 입력시

struct INPUT_PACKET {
	char type;
	bool bKeyDown;
	char input;
};

#endif 

using namespace std;

// 클라이언트 정보
struct g_clientInfo {
	SOCKET sock; // 소켓
	int id; // 소켓 ID
};

// 소켓 정보
struct g_sockInfo {
	SOCKET sock; // 소켓
	int id; // 소켓 ID

	// 소켓 정보 리턴 함수
	g_sockInfo* GetSockInfo() { return this; }
};

random_device rd;
mt19937 gen{ rd() };

int g_iId;
g_sockInfo* siSockInfo;
SOCKET sock;