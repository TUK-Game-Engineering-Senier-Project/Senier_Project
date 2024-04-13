// cpp �ۼ��� ���� ����Ʈ : https://behonestar.tistory.com/23
// �׽�Ʈ�� Enemy ������Ʈ�� ����� ����ϴ� �����Դϴ�.

#include <stdlib.h>
#include <stdio.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#pragma comment(lib, "lua54.lib")

// int �ʵ� ���� �������� �Լ�
int lua_getInt(lua_State* L, const char* c) {
    lua_getfield(L, -1, c);
    int i = lua_tointeger(L, -1);
    lua_pop(L, 1);
    return i;
}

// string �ʵ� ���� �������� �Լ�
char* lua_getString(lua_State* L, const char* c) {
    lua_getfield(L, -1, c);
    char* str = (char*) lua_tostring(L, -1);
    lua_pop(L, 1);
    return str;
}

// ���ο� ���� �����ϴ� �Լ�
void lua_newEnemy(lua_State* L, const char* name, int hp) {
    lua_getglobal(L, "EnemyObject");
    lua_getfield(L, -1, "new");

    lua_pushstring(L, name);
    lua_pushinteger(L, hp);
    lua_call(L, 2, 1);

    lua_remove(L, -2);
}

// ������� �޴� �Լ�
void lua_takeDamage(lua_State* L, const char* name, int damage) {
    // �ۼ� ����
}

// ----- main �Լ� ----- //

int main() {
    // lua ����
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // lua ���� �ε� ������ ����
    if (luaL_loadfile(L, "test.lua") || lua_pcall(L, 0, 0, 0)) {
        printf("error: %s", lua_tostring(L, -1));
        return -1;
    }

    // ���ο� �� ������Ʈ�� �����Ѵ�.
    lua_newEnemy(L, "Goblin", 100);

    // �� ������Ʈ�� ������ �޾ƿ´�.
    char* name = lua_getString(L, "name");
    int hp = lua_getInt(L, "hp");

    // �޾ƿ� ������ ����Ѵ�.
    printf("name : %s / hp : %d\n", name, hp);

    // (����) Goblin ������ 20 ������� �ش�
    // lua_takeDamage(L, "Goblin", 20);

    // lua �ݱ�
    lua_close(L);
    return 0;
}