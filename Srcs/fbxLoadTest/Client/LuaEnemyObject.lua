-- EnemyObject ���̺��� �����
EnemyObject = {}

-- ���ο� EnemyObject�� �����Ͽ� �ʱ�ȭ�Ѵ�.
function EnemyObject.new(name, hp, x, y, z)
  local self = {
    nowState = "Standing",
    name = name,
    hp = hp,
    x = x,
    y = y,
    z = z
  }

  -- �ش� EnemyObject�� name�� ���������� �����Ѵ�
  _G[name] = self

  return self
end

-- EnemyObject�� ������Ʈ�Ѵ�
function EnemyObject:update(state, x, y, z)
   self.state = state
   self.x = x
   self.y = y
   self.z = z
end

