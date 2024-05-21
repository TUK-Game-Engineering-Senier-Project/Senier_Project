// <Lua �� ������Ʈ ��� ����>

// Visual Studio���� (���� ���� ����)
// - C / C++ - �Ϲ� - �߰� ���� ���͸� ���� : include ��� C : \lua - 5.4.2_Win64_vc17_lib\include ����
// - �Ӽ� - ��Ŀ - �Ϲ� - �߰� ���̺귯�� ���͸� ���� : lub54.lib ��� C : \lua - 5.4.2_Win64_vc17_lib ����

// lua_State* L = luaL_newstate(); �� SoulSimul.cpp�� ����
// SoulSimul�� �����ڿ��� lua�� ���� �Ҹ��ڿ��� lua�� �ݴ´�

#pragma once

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#pragma comment(lib, "lua54.lib")

// ���ο� ���� �����ϴ� �Լ�
void lua_newEnemy(lua_State* L, const char* enemyName, int hp, float x, float y, float z);

// �� �ൿ Ʈ��
void lua_BehaviorTree(lua_State* L, const char* enemyName, float px, float pz);

// �� ���� ����
void lua_DoAction(lua_State* L, const char* enemyName, float px, float pz);

// LuaEnemyObject.lua�� �ִ� �Լ��� �����Ͽ� dataName (float) ���� �����´�
float lua_getFuncFloat(lua_State* L, const char* enemyName, const char* funcName, const char* dataName);





// --- ### �׽�Ʈ�� [���] ������ ǥ�� ��ġ ---
// --- ### ��� �Ϸ� �� ���� �� ---

#include <windows.h>
void LuaDebugOutput(const char* message);
int LuaDebugOutputWrapper(lua_State* L);
void lua_registerLuaDebugOutput(lua_State* L);

