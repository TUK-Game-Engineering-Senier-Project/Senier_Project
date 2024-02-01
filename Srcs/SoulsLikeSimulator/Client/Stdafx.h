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

#include "Objects.h" // 오브젝트 관련 내용 헤더

#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#endif 