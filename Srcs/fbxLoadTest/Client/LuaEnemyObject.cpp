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

	// push�� �� ������ ��ȯ���� 1���� �Լ��� ȣ���Ѵ�.
	lua_call(L, 5, 1);

	// ���ÿ��� EnemyObject ���̺��� �����Ѵ�.
	lua_pop(L, 1);
}

float lua_getX(lua_State* L, const char* name)
{
    // getX �Լ��� ���ÿ� �ø���
    lua_getglobal(L, "getX");

    // name�� ���ÿ� �ø���
    lua_pushstring(L, name);

	// name�� �ش��ϴ� ������Ʈ�� getX�� �����Ѵ�
	lua_pcall(L, 1, 1, 0);

    // �޾ƿ� ���� �����Ѵ�
    float x = static_cast<float>(lua_tonumber(L, -1));

    // ���ÿ� �ִ� �޾ƿ� ���� �����
    lua_pop(L, 1);

	// ���� ��ȯ�Ѵ�
    return x;
}

float lua_getY(lua_State* L, const char* name)
{
	// getY �Լ��� ���ÿ� �ø���
	lua_getglobal(L, "getY");

	// name�� ���ÿ� �ø���
	lua_pushstring(L, name);

	// name�� �ش��ϴ� ������Ʈ�� getX�� �����Ѵ�
	lua_pcall(L, 1, 1, 0);

	// �޾ƿ� ���� �����Ѵ�
	float y = static_cast<float>(lua_tonumber(L, -1));

	// ���ÿ� �ִ� �޾ƿ� ���� �����
	lua_pop(L, 1);

	// ���� ��ȯ�Ѵ�
	return y;
}

float lua_getZ(lua_State* L, const char* name)
{
	// getZ �Լ��� ���ÿ� �ø���
	lua_getglobal(L, "getZ");

	// name�� ���ÿ� �ø���
	lua_pushstring(L, name);

	// name�� �ش��ϴ� ������Ʈ�� getX�� �����Ѵ�
	lua_pcall(L, 1, 1, 0);

	// �޾ƿ� ���� �����Ѵ�
	float z = static_cast<float>(lua_tonumber(L, -1));

	// ���ÿ� �ִ� �޾ƿ� ���� �����
	lua_pop(L, 1);

	// ���� ��ȯ�Ѵ�
	return z;
}

// (����) �� ������ ������Ʈ�ϴ� �Լ� 
void lua_update(lua_State* L) {
	// ### �ۼ� ����
}