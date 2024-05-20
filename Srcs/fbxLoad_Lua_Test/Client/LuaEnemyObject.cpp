// <Lua 적 오브젝트 cpp 파일>

#include "LuaEnemyObject.h"

// 새로운 적을 생성하는 함수
void lua_newEnemy(lua_State* L, const char* name, int hp, float x, float y, float z) {

	// EnemyObject 테이블을 가져온다.
	lua_getglobal(L, "EnemyObject");

	// EnemyObject 테이블 내의 new 함수를 가져온다.
	lua_getfield(L, -1, "new");

	// name과 hp값을 push한다
	lua_pushstring(L, name);
	lua_pushinteger(L, hp);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);

	// 매개변수 5개와 반환값 1개를 가지는 함수를 실행한다
	lua_call(L, 5, 1);

	// 스택에서 EnemyObject 테이블을 제거한다
	lua_pop(L, 1);
}

// 적 행동 트리
void lua_BehaviorTree(lua_State* L, const char* enemyName, float px, float pz)
{
	// 함수 이름을 스택에 올린다
	lua_getglobal(L, "BehaviorTree");

	// 매개변수들을 스택에 올린다
	lua_pushstring(L, enemyName);
	lua_pushnumber(L, px);
	lua_pushnumber(L, pz);

	// 매개변수 3개와 반환값 1개를 가지는 함수를 실행한다
	lua_pcall(L, 3, 1, 0);
}

// 적 동작 수행
void lua_DoAction(lua_State* L, const char* enemyName)
{
	// 함수 이름을 스택에 올린다
	lua_getglobal(L, "DoAction");

	// 매개변수들을 스택에 올린다
	lua_pushstring(L, enemyName);

	// 매개변수 1개와 반환값 1개를 가지는 함수를 실행한다.
	lua_pcall(L, 1, 1, 0);
}

// LuaEnemyObject.lua에 있는 함수를 실행하여 dataName (float) 값을 가져온다
float lua_getFuncFloat(lua_State* L, const char* enemyName, const char* funcName, const char* dataName)
{
	// funcName을 스택에 올린다
	lua_getglobal(L, funcName);

	// enemyName을 스택에 올린다
	lua_pushstring(L, enemyName);

	// dataName을 스택에 올린다
	lua_pushstring(L, dataName);

	// name에 해당하는 오브젝트의 getData(enemyName, dataName)를 실행한다
	lua_pcall(L, 2, 1, 0);

	// 받아온 값을 저장한다
	float f = static_cast<float>(lua_tonumber(L, -1));

	// 스택에 있는 받아온 값을 지운다
	lua_pop(L, 1);

	// float 결과값을 반환한다
	return f;
}

// LuaEnemyObject.lua에 있는 lua_ifLookingPlayer 함수를 실행한다
bool lua_ifLookingPlayer(lua_State* L, const char* enemyName, const char* dir, float px, float pz, float areaLength) 
{
	// 함수 이름을 스택에 올린다
	lua_getglobal(L, "ifLookingPlayer");

	// 매개변수들을 스택에 올린다
	lua_pushstring(L, enemyName);
	lua_pushstring(L, dir);
	lua_pushnumber(L, px);
	lua_pushnumber(L, pz);
	lua_pushnumber(L, areaLength);

	// 매개변수 5개와 반환값 1개를 가지는 함수를 실행한다.
	lua_pcall(L, 5, 1, 0);

	// 받아온 값을 저장한다
	bool b = lua_toboolean(L, -1);

	// 스택에 있는 받아온 값을 지운다
	lua_pop(L, 1);

	// bool 결과값을 반환한다
	return b;
}