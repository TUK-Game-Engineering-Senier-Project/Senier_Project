# Lua 스크립트

### 참고 링크
- 다운로드 : https://www.lua.org/download.html
- 참고 링크 : https://hazarddev.tistory.com/34 ~ 36, https://reebok.tistory.com/80 ~ 83
- 메타 테이블 참고 : https://blog.naver.com/qkrqldtjqj/220565237699, https://delightlane.tistory.com/10
- Visual Studio와 연결 참고 : https://behonestar.tistory.com/23
---

필요한 내용 있으면 추가 예정

---

### 기본 내용

자료형 : 동적 타입 언어. Python처럼 입력받는 값에 따라 자료형이 결정된다.
a = 6 // 정수형
b = "6" // 문자형

print "Hello, world" 처럼 괄호를 씌우지 않고도 출력할 수 있다.
- 자기 스타일에 맞게 일관된 코드 작성을 권장

Python의 None이나 C의 NULL에 해당하는 값을 nil으로 작성한다.

// .md 파일이 {괄호 내부}가 안 보이길래 한글 변수명을 사용함
student = {이름 = nil, 학년 = nil, major = "GameEngineering"}
student.이름 = "Seungwan"
student.학년 = 4
처럼 테이블 형을 선언하고, nil로 선언된 값의 경우 외부에서 수정할 수 있다.

Lua에서는 테이블 인덱스가 0이 아닌 '1부터 시작한다'.

그 외 기본 내용 상세 : https://reebok.tistory.com/80

수학 함수들 : https://reebok.tistory.com/82

### 함수 작성
function funcname(n)
// 동작
end
이런 식으로 function 뒤에 함수 이름과 인자를 작성하며 end로 끝맺는다.

함수 내에서 사용하는 변수는 local a = 20 과 같이 local을 앞에 붙여 선언한다.

### 조건문
if 조건 then
// 동작
elseif 조건 then // else와 if를 붙여쓴다.
// 동작
else
// 동작
end

### 반복문

while 조건 do
// 동작
end

repeat 
// 동작
until 조건

while-do는 비교조건이 동작보다 앞서 실행된다. 
반면 repeat-until은 동작이 비교조건보다 앞서 실행된다 (do-while처럼).

generic한 for 루프 (for-in-do)
for key, value in pairs(tablename) do
print(key, value)
end
라고 작성하면 테이블 형의 key, value쌍을 전부 출력한다.

### 메타 테이블
setmetatable(table, metatable) // table의 메타 테이블을 설정한다.
getmetatable(table) // table의 메타 테이블을 가져온다.

기본 행동(변수의 기본값, 함수의 기본 행위 등)을 정의하는 테이블

__index 예제
t = {ㅇ} // 안 보이길래 ㅇ 넣음, 원래는 ㅇ 없는 코드
mt = {
    __index = function(t, k)
        return k.." 키로 호출됨"
    end
}
setmetatable(t, mt)
print(t[어떤값])

- 결과 : 어떤값 키로 호출됨

__newindex 예제
t = {ㅇ} // 안 보이길래 ㅇ 넣음, 원래는 ㅇ 없는 코드
mt = {
    __newindex = function(t, k, v)
        rawset(t, k, v)
        print(k.." = "..v.." 할당 완료")
    end
}
setmetatable(t, mt)
t[1] = "one"

- 결과 : 1 = one 할당 완료
- rawset()은 메타 테이블 콜스택을 거치지 않고 할당하는 기능을 한다 (무한루프 방지).

테이블과 연산자를 setmetatable로 결합하면 C++의 연산자 오버로딩과 유사한 기능을 한다.
- __add, __sub, __mul, __div 등등

// 메타 테이블은 아직 잘 모르겠어서 혹시 나중에 쓸 일이 있으면 더 확실히 알아볼 예정입니다.

### Visual Studio 개발 환경
- 참고 : https://behonestar.tistory.com/23

Lua 최신 버전을 다운받은 뒤 압축 해제
-> Visual Studio 프로젝트 생성
-> Header Files와 Source Files에 Lua의 .h와 .c파일 포함 (lua.c와 luah.c 제외)
-> Lua 스크립트 작성 후 Visual Studio 프로젝트에 저장
-> 프로젝트 main 함수 작성

// src 폴더에 넣은 경우
#include "src/lua.h"
#include "src/lualib.h"
#include "src/lauxlib.h"