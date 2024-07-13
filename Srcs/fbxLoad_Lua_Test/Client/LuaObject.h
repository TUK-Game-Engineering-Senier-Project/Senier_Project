// <Lua �� ������Ʈ ��� ����>

// Visual Studio���� (���� ���� ����)
// - C / C++ - �Ϲ� - �߰� ���� ���͸� ���� : include ��� C:\lua-5.4.2_Win64_vc17_lib\include ����
// - �Ӽ� - ��Ŀ - �Ϲ� - �߰� ���̺귯�� ���͸� ���� : lub54.lib ��� C:\lua-5.4.2_Win64_vc17_lib ����

// lua_State* L = luaL_newstate(); �� SoulSimul.cpp�� ����
// SoulSimul�� �����ڿ��� lua�� ���� �Ҹ��ڿ��� lua�� �ݴ´�

#pragma once

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#pragma comment(lib, "lua54.lib")

// ���ο� ������Ʈ�� �����ϴ� �Լ�
void lua_newObject(lua_State* L, const char* enemyName, int hp, float x, float y, float z);

// ������Ʈ ������Ʈ
void lua_updateObject(lua_State* L, const char* name, int hp, int px, int py, int pz, int rx, int ry, int rz);

// �� �ൿ Ʈ��
void lua_BehaviorTree(lua_State* L, const char* enemyName, const char* playerName, float px, float pz);

// �� ���� ����
void lua_DoAction(lua_State* L, const char* enemyName, const char* playerName, float px, float pz);

// LuaEnemyObject.lua�� �ִ� �Լ��� �����Ͽ� dataName (float) ���� �����´�
float lua_getFuncFloat(lua_State* L, const char* enemyName, const char* funcName, const char* dataName);

// LuaEnemyObject.lua�� �ִ� IfLookingPlayer �Լ��� �����Ѵ�
bool lua_ifLookingPlayer(lua_State* L, const char* enemyName, const char* dir, float px, float pz, float areaLength);


// --- ### �׽�Ʈ�� [���] ������ ǥ�� ��ġ ---
// --- ### ��� �Ϸ� �� ���� �� ---
/*
#include <windows.h>
void LuaDebugOutput(const char* message);
int LuaDebugOutputWrapper(lua_State* L);
void lua_registerLuaDebugOutput(lua_State* L);
*/