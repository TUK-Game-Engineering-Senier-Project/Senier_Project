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

-- x��ǥ ��������
function getX(name)
    local enemy = _G[name]
    if enemy then
        return enemy.x
    else
        return nil
    end
end

-- y��ǥ ��������
function getY(name)
    local enemy = _G[name]
    if enemy then
        return enemy.y
    else
        return nil
    end
end

-- z��ǥ ��������
function getZ(name)
    local enemy = _G[name]
    if enemy then
        return enemy.z
    else
        return nil
    end
end