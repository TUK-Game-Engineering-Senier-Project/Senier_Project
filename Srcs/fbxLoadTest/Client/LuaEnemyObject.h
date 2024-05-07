// <Lua 적 오브젝트 헤더 파일>

// Visual Studio에서 (본인 폴더 기준)
// - C / C++ - 일반 - 추가 포함 디렉터리 설정 : include 경로 C : \lua - 5.4.2_Win64_vc17_lib\include 지정
// - 속성 - 링커 - 일반 - 추가 라이브러리 디렉터리 설정 : lub54.lib 경로 C : \lua - 5.4.2_Win64_vc17_lib 지정

#pragma once

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#pragma comment(lib, "lua54.lib")

// 새로운 적을 생성하는 함수
void lua_newEnemy(lua_State* L, const char* name, int hp, float x, float y, float z);

// (예정) 적 정보를 업데이트하는 함수 
void lua_update(lua_State* L);