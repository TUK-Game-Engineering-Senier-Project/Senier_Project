#pragma once

#ifndef STDAFX_H_
#define STDAFX_H_

#include <iostream>
#include <random>
#include <thread>
#include <math.h>

#include <mutex> // 서버 구성용 뮤텍스 헤더 
#include <vector> // 벡터 리스트 생성용
// 오류 나서 찾아보니까 winsock.h(가 포함된) 헤더를
// Windows.h 위에 달아줘야 한다고 하여 위치 변경함
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더
#include <WindowsX.h>

#include "Objects.h" // 오브젝트 관련 내용 헤더

#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

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



extern constexpr char SC_PLAYER_MOVE = 0;
extern constexpr char SC_KEY_INPUT = 1;
extern constexpr char SC_PLAYER_ROTATE = 2;
extern constexpr char SC_SEND_PLAYER = 3;

// 입력 패킷
extern struct INPUT_PACKET
{
	char type;
	bool bKeyDown;
	char input;
};

// 이동 패킷
extern struct MOVE_PACKET
{
	char type; // 종류
	float fx, fy, fz; // 위치
	// float rot;
};

// 회전 패킷
extern struct ROTATE_PACKET
{
	char type;
	float fx, fy, fz;
};

// 플레이어 전송
extern struct SEND_PLAYER {
	char type;
	int id;
};

// 플레이어 구조체
extern struct Player
{
	float trans_x{}, trans_y{}, trans_z{};
	float scale_x{ 0.5f }, scale_y{ 1.0f }, scale_z{ 0.5f };
	// float rotate_x{}, rotate_y{}, rotate_z{};
	// float color_r{}, color_g{}, color_b{};
};

// 플레이어 구조체
extern struct PlayerLookDir
{
	float trans_x{}, trans_y{}, trans_z{};
	float scale_x{ 0.1f }, scale_y{ 0.1 }, scale_z{ 0.1f };
	// float rotate_x{}, rotate_y{}, rotate_z{};
	// float color_r{}, color_g{}, color_b{};
};

extern int g_id; // 플레이어 ID
extern Player player[2]; // 플레이어 2인
extern PlayerLookDir playerLookDir[2]; // 플레이어 각자 보는 방향

#endif 

