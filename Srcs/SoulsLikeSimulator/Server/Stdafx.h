#pragma once

// <사용자 정의 헤더 파일>

#ifndef STDAFX_H_ // 재정의 방지 if문 시작
#define STDAFX_H_

constexpr int SERVERPORT = 9000;     // 서버 포트 번호
constexpr int BUFSIZE = 512;         // 버퍼 크기

#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mutex> // 뮤텍스
#include <vector> // 벡터
#include <random> // 랜덤값

// #include <string>
// #include <stdio.h>
// #include <iostream>
// #include <chrono>

// #include "..\Protocol.h"
#include "GameObject.h" // 게임 오브젝트 헤더

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

// ----- 사용자 정의 값, 클래스 -----

extern float g_fPlayerSpd;  // 이동 속도
extern float g_fJumpSpd; // 점프 속도


// 클라이언트 정보
struct ClientInfo {
	SOCKET socket;
	int id;
};

// 소켓 정보
struct SOCK_INFO {
	SOCKET client_sock{};
	int id{};

	SOCK_INFO* GetSockInfo() { return this; }
};

constexpr char SC_PLAYER_MOVE = 0;
constexpr char SC_KEY_INPUT = 1;
constexpr char SC_PLAYER_ROTATE = 2;
constexpr char SC_SEND_PLAYER = 3;

// 입력 패킷
struct INPUT_PACKET {
	char type;
	bool bKeyDown;
	char input;
};

// 이동 패킷
struct MOVE_PACKET {
	char type;
	float fx, fy, fz;
	float rot;
};

// 회전 패킷
struct ROTATE_PACKET {
	char type;
	float fx, fy, fz;
};

// 플레이어 전송
struct SEND_PLAYER {
	char type;
	int id;
};

#endif