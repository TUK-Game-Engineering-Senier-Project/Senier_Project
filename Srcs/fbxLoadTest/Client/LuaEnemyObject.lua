-- EnemyObject 테이블을 만든다
EnemyObject = {}

-- 새로운 EnemyObject를 생성하여 초기화한다.
function EnemyObject.new(name, hp, x, y, z)
  local self = {
    nowState = "Standing",
    name = name,
    hp = hp,
    x = x,
    y = y,
    z = z
  }

  -- 해당 EnemyObject의 name을 전역변수로 저장한다
  _G[name] = self

  return self
end

-- EnemyObject를 업데이트한다
function EnemyObject:update(state, x, y, z)
   self.state = state
   self.x = x
   self.y = y
   self.z = z
end

