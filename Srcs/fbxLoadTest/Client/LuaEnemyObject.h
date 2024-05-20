// <Lua 적 오브젝트 헤더 파일>

// Visual Studio에서 (본인 폴더 기준)
// - C / C++ - 일반 - 추가 포함 디렉터리 설정 : include 경로 C : \lua - 5.4.2_Win64_vc17_lib\include 지정
// - 속성 - 링커 - 일반 - 추가 라이브러리 디렉터리 설정 : lub54.lib 경로 C : \lua - 5.4.2_Win64_vc17_lib 지정

// lua_State* L = luaL_newstate(); 는 SoulSimul.cpp에 있음
// 생성자에서 lua를 열고 소멸자에서 lua를 닫는다

#pragma once

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#pragma comment(lib, "lua54.lib")

// 새로운 적을 생성하는 함수
void lua_newEnemy(lua_State* L, const char* name, int hp, float x, float y, float z);

// LuaEnemyObject.lua에 있는 함수를 실행하여 dataName (float) 값을 가져온다
float lua_getFuncFloat(lua_State* L, const char* enemyName, const char* funcName, const char* dataName);

// LuaEnemyObject.lua에 있는 함수를 실행한다
void lua_doFunc(lua_State* L, const char* enemyName, const char* funcName);