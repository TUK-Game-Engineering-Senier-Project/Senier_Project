// cpp 작성시 참고 사이트 : https://behonestar.tistory.com/23
// 테스트용 Enemy 오브젝트를 만들고 사용하는 예제입니다.

#include <stdlib.h>
#include <stdio.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#pragma comment(lib, "lua54.lib")

// int 필드 값을 가져오는 함수
int lua_getInt(lua_State* L, const char* c) {
    lua_getfield(L, -1, c);
    int i = lua_tointeger(L, -1);
    lua_pop(L, 1);
    return i;
}

// string 필드 값을 가져오는 함수
char* lua_getString(lua_State* L, const char* c) {
    lua_getfield(L, -1, c);
    char* str = (char*) lua_tostring(L, -1);
    lua_pop(L, 1);
    return str;
}

// 새로운 적을 생성하는 함수
void lua_newEnemy(lua_State* L, const char* name, int hp) {
    lua_getglobal(L, "EnemyObject");
    lua_getfield(L, -1, "new");

    lua_pushstring(L, name);
    lua_pushinteger(L, hp);
    lua_call(L, 2, 1);

    lua_remove(L, -2);
}

// 대미지를 받는 함수
void lua_takeDamage(lua_State* L, const char* name, int damage) {
    // 작성 예정
}

// ----- main 함수 ----- //

int main() {
    // lua 열기
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // lua 파일 로딩 오류시 종료
    if (luaL_loadfile(L, "test.lua") || lua_pcall(L, 0, 0, 0)) {
        printf("error: %s", lua_tostring(L, -1));
        return -1;
    }

    // 새로운 적 오브젝트를 생성한다.
    lua_newEnemy(L, "Goblin", 100);

    // 적 오브젝트의 정보를 받아온다.
    char* name = lua_getString(L, "name");
    int hp = lua_getInt(L, "hp");

    // 받아온 정보를 출력한다.
    printf("name : %s / hp : %d\n", name, hp);

    // (예정) Goblin 적에게 20 대미지를 준다
    // lua_takeDamage(L, "Goblin", 20);

    // lua 닫기
    lua_close(L);
    return 0;
}