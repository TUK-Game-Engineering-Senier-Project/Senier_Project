// <Lua �� ������Ʈ cpp ����>

#include "LuaEnemyObject.h"

// ���ο� ���� �����ϴ� �Լ�
void lua_newEnemy(lua_State* L, const char* name, int hp, float x, float y, float z) {

	// EnemyObject ���̺��� �����´�.
	lua_getglobal(L, "EnemyObject");

	// EnemyObject ���̺� ���� new �Լ��� �����´�.
	lua_getfield(L, -1, "new");

	// name�� hp���� push�Ѵ�
	lua_pushstring(L, name);
	lua_pushinteger(L, hp);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pushnumber(L, z);

	// �Ű����� 5���� ��ȯ�� 1���� ������ �Լ��� �����Ѵ�
	lua_call(L, 5, 1);

	// ���ÿ��� EnemyObject ���̺��� �����Ѵ�
	lua_pop(L, 1);
}

// �� �ൿ Ʈ��
void lua_BehaviorTree(lua_State* L, const char* enemyName, float px, float pz)
{
	// �Լ� �̸��� ���ÿ� �ø���
	lua_getglobal(L, "BehaviorTree");

	// �Ű��������� ���ÿ� �ø���
	lua_pushstring(L, enemyName);
	lua_pushnumber(L, px);
	lua_pushnumber(L, pz);

	// �Ű����� 3���� ��ȯ�� 1���� ������ �Լ��� �����Ѵ�
	lua_pcall(L, 3, 1, 0);
}

// �� ���� ����
void lua_DoAction(lua_State* L, const char* enemyName)
{
	// �Լ� �̸��� ���ÿ� �ø���
	lua_getglobal(L, "DoAction");

	// �Ű��������� ���ÿ� �ø���
	lua_pushstring(L, enemyName);

	// �Ű����� 1���� ��ȯ�� 1���� ������ �Լ��� �����Ѵ�.
	lua_pcall(L, 1, 1, 0);
}

// LuaEnemyObject.lua�� �ִ� �Լ��� �����Ͽ� dataName (float) ���� �����´�
float lua_getFuncFloat(lua_State* L, const char* enemyName, const char* funcName, const char* dataName)
{
	// funcName�� ���ÿ� �ø���
	lua_getglobal(L, funcName);

	// enemyName�� ���ÿ� �ø���
	lua_pushstring(L, enemyName);

	// dataName�� ���ÿ� �ø���
	lua_pushstring(L, dataName);

	// name�� �ش��ϴ� ������Ʈ�� getData(enemyName, dataName)�� �����Ѵ�
	lua_pcall(L, 2, 1, 0);

	// �޾ƿ� ���� �����Ѵ�
	float f = static_cast<float>(lua_tonumber(L, -1));

	// ���ÿ� �ִ� �޾ƿ� ���� �����
	lua_pop(L, 1);

	// float ������� ��ȯ�Ѵ�
	return f;
}

// LuaEnemyObject.lua�� �ִ� lua_ifLookingPlayer �Լ��� �����Ѵ�
bool lua_ifLookingPlayer(lua_State* L, const char* enemyName, const char* dir, float px, float pz, float areaLength) 
{
	// �Լ� �̸��� ���ÿ� �ø���
	lua_getglobal(L, "ifLookingPlayer");

	// �Ű��������� ���ÿ� �ø���
	lua_pushstring(L, enemyName);
	lua_pushstring(L, dir);
	lua_pushnumber(L, px);
	lua_pushnumber(L, pz);
	lua_pushnumber(L, areaLength);

	// �Ű����� 5���� ��ȯ�� 1���� ������ �Լ��� �����Ѵ�.
	lua_pcall(L, 5, 1, 0);

	// �޾ƿ� ���� �����Ѵ�
	bool b = lua_toboolean(L, -1);

	// ���ÿ� �ִ� �޾ƿ� ���� �����
	lua_pop(L, 1);

	// bool ������� ��ȯ�Ѵ�
	return b;
}