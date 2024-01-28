#pragma once

// <사용자 정의 헤더 파일>

#ifndef STDAFX_H_ // 재정의 방지 if문 시작
#define STDAFX_H_

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

#endif // 재정의 방지 if문 종료

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
