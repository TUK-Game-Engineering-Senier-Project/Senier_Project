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

// (예정) 적 정보를 업데이트하는 함수 
void lua_update(lua_State* L) {
	// ### 작성 예정
}