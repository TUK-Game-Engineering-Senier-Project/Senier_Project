#pragma once

#ifndef STDAFX_H_
#define STDAFX_H_

#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <vector>
#include <mutex>      // 서버 구성용 뮤텍스 헤더
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "ws2_32") 

// 클라이언트 정보
struct g_clientInfo
{
	SOCKET sock; // 소켓
	int id; // 소켓 ID
};

// 소켓 정보
struct g_sockInfo
{
	SOCKET sock; // 소켓
	int id; // 소켓 ID

	// 소켓 정보 리턴 함수
	g_sockInfo* GetSockInfo() { return this; }
};

// 소켓 정보
extern constexpr char SC_UPDATE = 10;
extern constexpr char SC_PLAYER_MOVE = 0;
extern constexpr char SC_KEY_INPUT = 1;
extern constexpr char SC_PLAYER_ROTATE = 2;
extern constexpr char SC_SEND_PLAYER = 3;

// 적 동작
extern constexpr char ACTION_NONE = 100; // 정지 상태
extern constexpr char ACTION_MOVETOPLAYER = 101; // 플레이어를 향해 이동
extern constexpr char ACTION_ATTACKPLAYER = 102; // 플레이어 공격

// 입력 패킷
struct INPUT_PACKET
{
	char type;
	bool bKeyDown;
	char input;
};

// 이동 패킷
struct MOVE_PACKET
{
	char cType; // 종류
	float fPx, fPy, fPz; // 위치
	float fRx, fRy, fRz; // 회전
	float fMx, fMy, fMz; // 이동

	float fRadius; // 시야 위치

	bool bNowMoving{ false }; // 현재 이동중 여부

	char cNowAction{ ACTION_NONE }; // 현재 동작
};


// 회전 패킷
struct ROTATE_PACKET
{
	char type;
	float fx, fy, fz;
};

// 플레이어 전송
struct SEND_PLAYER 
{
	char type;
	int id;
};

/* ----- 여기부터 추가된 부분 ----- */

// 오브젝트 구조체

extern struct Object
{
	float scale_x{ 1.0f }, scale_y{ 1.0f }, scale_z{ 1.0f };    // 크기 배율
	float rotate_x{ 0.0f }, rotate_y{ 0.0f }, rotate_z{ 0.0f }; // 회전 각도
	float pos_x{ 0.0f }, pos_y{ 0.0f }, pos_z{ 0.0f };          // 위치

	// '플레이어' 오브젝트용
	bool bNowMoving{ false }; // 현재 이동중 여부

	// '적' 오브젝트용
	char cNowAction{ ACTION_NONE }; // 현재 동작
};

// 이 부분의 본래 선언 위치는 d3dApp.cpp이다.
extern int g_id; // 플레이어 ID
extern Object floorobj;  // 바닥
extern Object player[2]; // 플레이어 2인
extern Object enemy_s;   // 소형 적
extern Object enemy_m;   // 중형 적
extern Object enemy_l;   // 대형 적

#endif 

