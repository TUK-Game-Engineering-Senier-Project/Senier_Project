-- lua 작성시 참고 사이트 : https://exqt.me/lua-class/
-- 테스트용 Enemy 오브젝트를 만들고 사용하는 예제입니다.

-- EnemyObject 테이블을 만든다
EnemyObject = {}

-- 새로운 EnemyObject를 생성하여 초기화한다.
function EnemyObject.new(name, hp)
  local self = {
    nowState = "Standing",
    name = name,
    hp = hp
  }

  -- 해당 EnemyObject의 name을 전역변수로 저장한다.
  _G[name] = self

  return self
end

-- EnemyObject의 동작 : 대미지를 받는다.
function EnemyObject:TakeDamage(amount)
    self.hp = self.hp - amount
end