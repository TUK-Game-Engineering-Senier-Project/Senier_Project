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

// (����) �� ������ ������Ʈ�ϴ� �Լ� 
void lua_update(lua_State* L) {
	// ### �ۼ� ����
}