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

	// push한 두 값으로 반환값이 1개인 함수를 호출한다.
	lua_call(L, 5, 1);

	// 스택에서 EnemyObject 테이블을 제거한다.
	lua_pop(L, 1);
}

#include <windows.h> 

float lua_getX(lua_State* L, const char* name)
{
    // getX 함수를 스택에 올린다
    lua_getglobal(L, "GetX");

    // name을 스택에 올린다
    lua_pushstring(L, name);

	// name에 해당하는 오브젝트의 getX를 실행한다
	lua_pcall(L, 1, 1, 0);

    // 받아온 값을 저장한다
    float x = static_cast<float>(lua_tonumber(L, -1));

    // 스택에 있는 받아온 값을 지운다
    lua_pop(L, 1);

	// 값을 반환한다
    return x;
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

	// 값을 반환한다
	return f;
}

// LuaEnemyObject.lua에 있는 함수를 실행한다
void lua_doFunc(lua_State* L, const char* enemyName, const char* funcName)
{
	// funcName 함수를 스택에 올린다
	lua_getglobal(L, funcName);

	// enemyName을 스택에 올린다
	lua_pushstring(L, enemyName);

	// enemyName에 해당하는 오브젝트의 funcName 함수를 실행한다
	lua_pcall(L, 1, 0, 0);
}