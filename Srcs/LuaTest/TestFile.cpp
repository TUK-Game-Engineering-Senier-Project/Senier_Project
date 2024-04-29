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

    // (�ʵ�� == c) �ʵ带 �����´�.
    lua_getfield(L, -1, c);

    // int ���� �����Ѵ�.
    int i = lua_tointeger(L, -1);

    // ������ �ʵ带 �����Ѵ�.
    lua_pop(L, 1);

    // int ���� ��ȯ�Ѵ�.
    return i;
}

// string �ʵ� ���� �������� �Լ�
char* lua_getString(lua_State* L, const char* c) {

    // (�ʵ�� == c) �ʵ带 �����´�.
    lua_getfield(L, -1, c);

    // char* ���� �����Ѵ�.
    char* str = (char*) lua_tostring(L, -1);

    // ������ �ʵ带 �����Ѵ�.
    lua_pop(L, 1);

    // char* ���� ��ȯ�Ѵ�.
    return str;
}

// ���ο� ���� �����ϴ� �Լ�
void lua_newEnemy(lua_State* L, const char* name, int hp) {

    // EnemyObject ���̺��� �����´�.
    lua_getglobal(L, "EnemyObject");

    // EnemyObject ���̺� ���� new �Լ��� �����´�.
    lua_getfield(L, -1, "new");

    // name�� hp���� push�Ѵ�
    lua_pushstring(L, name);
    lua_pushinteger(L, hp);

    // push�� �� ������ ��ȯ���� 1���� �Լ��� ȣ���Ѵ�.
    lua_call(L, 2, 1);

    // ���ÿ��� EnemyObject ���̺��� �����Ѵ�.
    lua_pop(L, 1);
}

// ������� �޴� �Լ�
void lua_takeDamage(lua_State* L, const char* name, int damage) {

    // ���޵� ���� �̸����� �ش��ϴ� ������Ʈ�� ã�´�.
    lua_getglobal(L, name);

    // EnemyObject ���̺��� �����´�.
    lua_getglobal(L, "EnemyObject");

    // EnemyObject ���̺� ���� TakeDamage �Լ��� �����´�.
    lua_getfield(L, -1, "TakeDamage");

    // ������ ������ �� ������Ʈ�� ���ÿ� �ִ´�.
    lua_pushvalue(L, -3);
    lua_pushinteger(L, damage);

    // ������ �� ������ ��ȯ���� ���� �Լ��� �����Ѵ�.
    lua_call(L, 2, 0);

    // ���ÿ��� TakeDamage �Լ��� EnemyObject ���̺��� �����Ѵ�.
    lua_pop(L, 2);
}

// ----- enemy Ŭ���� ----- //

class EnemyClass {
private:
    char* name;
    int hp;

public:

    // ������ (�⺻)
    EnemyClass() : name(nullptr), hp(0) {} 

    // ������ (�� �Է�)
    EnemyClass(char* name, int hp) {
        this->name = name;
        this->hp = hp;
    }

    // ��� �� ����
    void setName(char* name, int hp) {
        this->name = name;
    }
    void setHp(int hp) {
        this->hp = hp;
    }

    // ��� �� ��������
    char* getName() {
        return this->name;
    }
    int getHp() {
        return this->hp;
    }
};

EnemyClass enemylist[3] = { };

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
    lua_newEnemy(L, "Slime", 10);

    // �� ������Ʈ�� ������ �޾ƿ´�.
    lua_getglobal(L, "Goblin"); // ���
    enemylist[0] = EnemyClass(lua_getString(L, "name"), lua_getInt(L, "hp"));
    lua_getglobal(L, "Slime"); // ������
    enemylist[1] = EnemyClass(lua_getString(L, "name"), lua_getInt(L, "hp"));

    // �޾ƿ� ������ ����Ѵ�.
    printf("name : %s / hp : %d\n", enemylist[0].getName(), enemylist[0].getHp());
    printf("name : %s / hp : %d\n", enemylist[1].getName(), enemylist[1].getHp());

    // ������ ������� �ش�.
    lua_takeDamage(L, "Goblin", 20);
    lua_takeDamage(L, "Slime", 9);

    // ���ŵ� ������ �޾ƿ´�.
    lua_getglobal(L, "Goblin"); // ���
    enemylist[0] = EnemyClass(lua_getString(L, "name"), lua_getInt(L, "hp"));
    lua_getglobal(L, "Slime"); // ������
    enemylist[1] = EnemyClass(lua_getString(L, "name"), lua_getInt(L, "hp"));

    // ���ŵ� ������ ����Ѵ�.
    printf("name : %s / hp : %d\n", enemylist[0].getName(), enemylist[0].getHp());
    printf("name : %s / hp : %d\n", enemylist[1].getName(), enemylist[1].getHp());

    // lua �ݱ�
    lua_close(L);
    return 0;
}