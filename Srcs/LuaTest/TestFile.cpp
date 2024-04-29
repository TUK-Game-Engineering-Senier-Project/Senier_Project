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

    // (필드명 == c) 필드를 가져온다.
    lua_getfield(L, -1, c);

    // int 값을 저장한다.
    int i = lua_tointeger(L, -1);

    // 가져온 필드를 제거한다.
    lua_pop(L, 1);

    // int 값을 반환한다.
    return i;
}

// string 필드 값을 가져오는 함수
char* lua_getString(lua_State* L, const char* c) {

    // (필드명 == c) 필드를 가져온다.
    lua_getfield(L, -1, c);

    // char* 값을 저장한다.
    char* str = (char*) lua_tostring(L, -1);

    // 가져온 필드를 제거한다.
    lua_pop(L, 1);

    // char* 값을 반환한다.
    return str;
}

// 새로운 적을 생성하는 함수
void lua_newEnemy(lua_State* L, const char* name, int hp) {

    // EnemyObject 테이블을 가져온다.
    lua_getglobal(L, "EnemyObject");

    // EnemyObject 테이블 내의 new 함수를 가져온다.
    lua_getfield(L, -1, "new");

    // name과 hp값을 push한다
    lua_pushstring(L, name);
    lua_pushinteger(L, hp);

    // push한 두 값으로 반환값이 1개인 함수를 호출한다.
    lua_call(L, 2, 1);

    // 스택에서 EnemyObject 테이블을 제거한다.
    lua_pop(L, 1);
}

// 대미지를 받는 함수
void lua_takeDamage(lua_State* L, const char* name, int damage) {

    // 전달된 적의 이름으로 해당하는 오브젝트를 찾는다.
    lua_getglobal(L, name);

    // EnemyObject 테이블을 가져온다.
    lua_getglobal(L, "EnemyObject");

    // EnemyObject 테이블 내의 TakeDamage 함수를 가져온다.
    lua_getfield(L, -1, "TakeDamage");

    // 이전에 가져온 적 오브젝트를 스택에 넣는다.
    lua_pushvalue(L, -3);
    lua_pushinteger(L, damage);

    // 가져온 두 값으로 반환값이 없는 함수를 실행한다.
    lua_call(L, 2, 0);

    // 스택에서 TakeDamage 함수와 EnemyObject 테이블을 제거한다.
    lua_pop(L, 2);
}

// ----- enemy 클래스 ----- //

class EnemyClass {
private:
    char* name;
    int hp;

public:

    // 생성자 (기본)
    EnemyClass() : name(nullptr), hp(0) {} 

    // 생성자 (값 입력)
    EnemyClass(char* name, int hp) {
        this->name = name;
        this->hp = hp;
    }

    // 멤버 값 지정
    void setName(char* name, int hp) {
        this->name = name;
    }
    void setHp(int hp) {
        this->hp = hp;
    }

    // 멤버 값 가져오기
    char* getName() {
        return this->name;
    }
    int getHp() {
        return this->hp;
    }
};

EnemyClass enemylist[3] = { };

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
    lua_newEnemy(L, "Slime", 10);

    // 적 오브젝트의 정보를 받아온다.
    lua_getglobal(L, "Goblin"); // 고블린
    enemylist[0] = EnemyClass(lua_getString(L, "name"), lua_getInt(L, "hp"));
    lua_getglobal(L, "Slime"); // 슬라임
    enemylist[1] = EnemyClass(lua_getString(L, "name"), lua_getInt(L, "hp"));

    // 받아온 정보를 출력한다.
    printf("name : %s / hp : %d\n", enemylist[0].getName(), enemylist[0].getHp());
    printf("name : %s / hp : %d\n", enemylist[1].getName(), enemylist[1].getHp());

    // 적에게 대미지를 준다.
    lua_takeDamage(L, "Goblin", 20);
    lua_takeDamage(L, "Slime", 9);

    // 갱신된 정보를 받아온다.
    lua_getglobal(L, "Goblin"); // 고블린
    enemylist[0] = EnemyClass(lua_getString(L, "name"), lua_getInt(L, "hp"));
    lua_getglobal(L, "Slime"); // 슬라임
    enemylist[1] = EnemyClass(lua_getString(L, "name"), lua_getInt(L, "hp"));

    // 갱신된 정보를 출력한다.
    printf("name : %s / hp : %d\n", enemylist[0].getName(), enemylist[0].getHp());
    printf("name : %s / hp : %d\n", enemylist[1].getName(), enemylist[1].getHp());

    // lua 닫기
    lua_close(L);
    return 0;
}