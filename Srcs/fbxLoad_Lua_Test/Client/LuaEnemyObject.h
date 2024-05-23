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

// ### 위치 값 구조체 (삭제 예정)
struct Position
{
	float x;
	float y;
	float z;

	Position(float in_x, float in_y, float in_z) {
		x = in_x;
		y = in_y;
		z = in_z;
	}
};

// 새로운 적을 생성하는 함수
void lua_newEnemy(lua_State* L, const char* enemyName, int hp, float x, float y, float z);

// 적 행동 트리
void lua_BehaviorTree(lua_State* L, const char* enemyName, float px, float pz);

// 적 동작 수행
void lua_DoAction(lua_State* L, const char* enemyName, float px, float pz);

// LuaEnemyObject.lua에 있는 함수를 실행하여 dataName (float) 값을 가져온다
float lua_getFuncFloat(lua_State* L, const char* enemyName, const char* funcName, const char* dataName);

// LuaEnemyObject.lua에 있는 IfLookingPlayer 함수를 실행한다
bool lua_ifLookingPlayer(lua_State* L, const char* enemyName, const char* dir, float px, float pz, float areaLength);

// LuaEnemyObject.lua에 있는 GetPlayerPosition 함수를 실행한다
void lua_getPlayerPosition(lua_State* L, Position& playerPosition);

// LuaEnemyObject.lua에 있는 UpdatePlayerPosition 함수를 실행한다
void lua_updatePlayerPosition(lua_State* L, float x, float y, float z);

// --- ### 테스트용 [출력] 데이터 표시 장치 ---
// --- ### 사용 완료 후 지울 것 ---
#include <windows.h>
void LuaDebugOutput(const char* message);
int LuaDebugOutputWrapper(lua_State* L);
void lua_registerLuaDebugOutput(lua_State* L);