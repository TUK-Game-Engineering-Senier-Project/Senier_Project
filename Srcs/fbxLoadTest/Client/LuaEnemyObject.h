// <Lua �� ������Ʈ ��� ����>

// Visual Studio���� (���� ���� ����)
// - C / C++ - �Ϲ� - �߰� ���� ���͸� ���� : include ��� C : \lua - 5.4.2_Win64_vc17_lib\include ����
// - �Ӽ� - ��Ŀ - �Ϲ� - �߰� ���̺귯�� ���͸� ���� : lub54.lib ��� C : \lua - 5.4.2_Win64_vc17_lib ����

// lua_State* L = luaL_newstate(); �� SoulSimul.cpp�� ����
// �����ڿ��� lua�� ���� �Ҹ��ڿ��� lua�� �ݴ´�

#pragma once

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#pragma comment(lib, "lua54.lib")

// ���ο� ���� �����ϴ� �Լ�
void lua_newEnemy(lua_State* L, const char* name, int hp, float x, float y, float z);

// �� ��ġ ������ �������� �Լ�
float lua_getX(lua_State* L, const char* name);
float lua_getY(lua_State* L, const char* name);
float lua_getZ(lua_State* L, const char* name);

// LuaEnemyObject.lua�� �ִ� �Լ� ����
void lua_doFunction(lua_State* L, const char* enemyName, const char* funcName);