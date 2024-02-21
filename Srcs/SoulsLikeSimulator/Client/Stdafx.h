#pragma once

#ifndef STDAFX_H_
#define STDAFX_H_

#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <mutex> // 서버 구성용 뮤텍스 헤더 
// 오류 나서 찾아보니까 winsock.h(가 포함된) 헤더를
// Windows.h 위에 달아줘야 한다고 하여 위치 변경함
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

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
struct INPUT_PACKET
{
	char type;
	bool bKeyDown;
	char input;
};

// 이동 패킷
struct MOVE_PACKET
{
	char type; // 종류
	float fPx, fPy, fPz; // 위치
	float fRx, fRy, fRz; // 회전
	float fMx, fMy, fMz; // 이동
};;


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

// 플레이어 구조체
extern struct Player
{
	float pos_x{ 0.0f },    pos_y{ 0.0f },    pos_z{ 0.0f };    // 위치
	float scale_x{ 0.3f },  scale_y{ 0.3f },  scale_z{ 0.3f };  // 크기 배율
	float rotate_x{ 0.0f }, rotate_y{ 0.0f }, rotate_z{ 0.0f }; // 회전 각도
	float move_x{ 0.0f },   move_y{ 0.0f },   move_z{ 0.0f };   // 이동 거리
	// float color_r{}, color_g{}, color_b{};
};

extern int g_id; // 플레이어 ID
extern Player player[2]; // 플레이어 2인
extern Player playerLookDir[2]; // 플레이어 각자 보는 방향

#endif 

