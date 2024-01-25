#pragma once

#ifndef STDAFX_H_
#define STDAFX_H_

#include <mutex> // 서버 구성용 뮤텍스 헤더 
#include <vector> // 벡터 리스트 생성용
#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include "../Common/d3dApp.h"
#include "../Common/MathHelper.h"
#include "../Common/UploadBuffer.h"

#include "Objects.h" // 오브젝트 관련 내용 헤더

#define SERVERPORT 9000 // 서버 포트 번호
#define BUFSIZE    512  // 버퍼 크기

constexpr char SC_KEY_INPUT = 1; // 키 입력시

struct INPUT_PACKET {
	char type;
	bool bKeyDown;
	char input;
};

#endif 