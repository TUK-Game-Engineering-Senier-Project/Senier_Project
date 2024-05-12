-- EnemyObject ���̺��� �����
EnemyObject = {}

-- ����� 
local PI = 3.141592654 -- ������
local LOOKINGAREA = 8.0 -- �� �þ�
local ATTACKAREA = 2.2 -- �� ���� ����
local DIROFFSET = 0.24 -- �� ���� ����
local DEFAULTWALKLENGTH = 0.005 -- ������ �� ���� �̵��ϴ� �Ÿ�
local FOLLOWWALKLENGTH = 0.012 -- �� ���� �̵��ϴ� �Ÿ�

-- ���ο� EnemyObject�� �����Ͽ� �ʱ�ȭ�Ѵ�.
function EnemyObject.new(name, hp, x, y, z)
  local self = {
    nowState = "DEFAULT",
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

-- x��ǥ ��������
function GetX(name)
    local enemy = _G[name]
    if enemy then
        return enemy.x
    else
        return nil
    end
end

-- y��ǥ ��������
function GetY(name)
    local enemy = _G[name]
    if enemy then
        return enemy.y
    else
        return nil
    end
end

-- z��ǥ ��������
function GetZ(name)
    local enemy = _G[name]
    if enemy then
        return enemy.z
    else
        return nil
    end
end

-- �ൿ Ʈ��
function BehaviorTree(name)
    local enemy = _G[name]

    -- (����) ���� ���� ���� �ִٸ� �÷��̾ �����Ѵ�
    if false then
        enemy.nowState = "ATTACKPLAYER"

    -- (����) �÷��̾ �ٶ󺸰� �ִٸ� (��и� �þ� �ȿ� �ִٸ�)
	-- �÷��̾�� �̵��Ѵ�
    elseif false then
        enemy.nowState = "MOVETOPLAYER"

    -- Default (�⺻ : ���) ������ �� �������� �����Ѵ�
    else
        enemy.nowState = "DEFAULT"

    end
end

-- �ൿġ
local behaviorpoint = 0

-- �� ���� ����
function DoAction(name)
    local enemy = _G[name]
	 
    -- ���� ���ۿ� ���� ���� ����

    if enemy.nowState == "ATTACKPLAYER" then
        enemy.nowState = "ATTACKPLAYER"

    elseif enemy.nowState == "MOVETOPLAYER" then
        enemy.nowState = "MOVETOPLAYER"

    elseif enemy.nowState == "DEFAULT" then
        
        -- �⺻ (���) ���� (### �ӽ�)
		-- ��->������->�Ʒ�->���� ���� ����(?) ���� ����

		-- �ൿġ�� �ൿġ�� ���� ����
		if behaviorpoint < 2000 then 
            behaviorpoint = behaviorpoint + 1
		else 
            behaviorpoint = 0
        end

        -- ����
		if behaviorpoint >= 1500 then 
			-- rotate_y = 1.5 * XM_PI;
			enemy.x = enemy.x - DEFAULTWALKLENGTH;

        -- �Ʒ�
		elseif behaviorpoint >= 1000 then 
			-- rotate_y = 1.0 * XM_PI;
			enemy.z = enemy.z - DEFAULTWALKLENGTH;

        -- ������
		elseif behaviorpoint >= 500 then
			-- rotate_y = 0.5 * XM_PI;
			enemy.x = enemy.x + DEFAULTWALKLENGTH;

		-- ��
		else
			-- rotate_y = 0.0 * XM_PI;
			enemy.z = enemy.z + DEFAULTWALKLENGTH;

        end
    end
end