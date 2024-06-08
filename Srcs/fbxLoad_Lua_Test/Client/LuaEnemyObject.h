// <Lua 적 오브젝트 헤더 파일>

// Visual Studio에서 (본인 폴더 기준)
// - C / C++ - 일반 - 추가 포함 디렉터리 설정 : include 경로 C : \lua - 5.4.2_Win64_vc17_lib\include 지정
// - 속성 - 링커 - 일반 - 추가 라이브러리 디렉터리 설정 : lub54.lib 경로 C : \lua - 5.4.2_Win64_vc17_lib 지정

// lua_State* L = luaL_newstate(); 는 SoulSimul.cpp에 있음
// SoulSimul의 생성자에서 lua를 열고 소멸자에서 lua를 닫는다

#pragma once

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#pragma comment(lib, "lua54.lib")

// 새로운 오브젝트를 생성하는 함수
void lua_newObject(lua_State* L, const char* enemyName, int hp, float x, float y, float z);

// 오브젝트 업데이트
void lua_updateObject(lua_State* L, const char* name, int hp, int px, int py, int pz, int rx, int ry, int rz);

// 적 행동 트리
void lua_BehaviorTree(lua_State* L, const char* enemyName, float px, float pz);

// 적 동작 수행
void lua_DoAction(lua_State* L, const char* enemyName, float px, float pz);

// LuaEnemyObject.lua에 있는 함수를 실행하여 dataName (float) 값을 가져온다
float lua_getFuncFloat(lua_State* L, const char* enemyName, const char* funcName, const char* dataName);

// LuaEnemyObject.lua에 있는 IfLookingPlayer 함수를 실행한다
bool lua_ifLookingPlayer(lua_State* L, const char* enemyName, const char* dir, float px, float pz, float areaLength);


// --- ### 테스트용 [출력] 데이터 표시 장치 ---
// --- ### 사용 완료 후 지울 것 ---
#include <windows.h>
void LuaDebugOutput(const char* message);
int LuaDebugOutputWrapper(lua_State* L);
void lua_registerLuaDebugOutput(lua_State* L);