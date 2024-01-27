#pragma once

// <서버 기능 헤더 파일>

// 클라이언트와 데이터 통신 함수
DWORD WINAPI ProcessClient(LPVOID arg);

// 키 정보 전송 함수
void SendKeyInput(g_sockInfo* sock_info, char input, bool KeyDown);

// 플레이어 정보 업데이트 함수
void UpdatePlayer();