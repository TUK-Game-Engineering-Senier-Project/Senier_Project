-- lua �ۼ��� ���� ����Ʈ : https://exqt.me/lua-class/
-- �׽�Ʈ�� Enemy ������Ʈ�� ����� ����ϴ� �����Դϴ�.

-- EnemyObject ���̺��� �����
EnemyObject = {}

-- ���ο� EnemyObject�� �����Ͽ� �ʱ�ȭ�Ѵ�.
function EnemyObject.new(name, hp)
  local self = {
    nowState = "Standing",
    name = name,
    hp = hp
  }
  return self
end

-- EnemyObject�� ���� : ������� �޴´�.
function EnemyObject:TakeDamage(amount)
    -- �߰� ����
end