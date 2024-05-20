-- EnemyObject ���̺��� �����
EnemyObject = {}

-- ����� 
local PI = 3.141592654 -- ������
local LOOKINGAREA = 8.0 -- �� �þ�
local ATTACKAREA = 2.2 -- �� ���� ����
local DIROFFSET = 0.24 -- �� ���� ����
local DEFAULTWALKLENGTH = 0.005 -- ������ �� ���� �̵��ϴ� �Ÿ�
local FOLLOWWALKLENGTH = 0.012 -- �� ���� �̵��ϴ� �Ÿ�

-- [[���ο� EnemyObject�� �����Ͽ� �ʱ�ȭ]]

function EnemyObject.new(name, hp, x, y, z)
  local self = {
    nowState = "DEFAULT",
    dir = "DOWN",
    name = name,
    hp = hp,
    pos_x = x,
    pos_y = y,
    pos_z = z,
    rotate_x = 0,
    rotate_y = 0,
    rotate_z = 0
  }

  -- �ش� EnemyObject�� name�� ������
  -- EnemyObject�� ���� ������Ʈ�� �����Ѵ�
  _G[name] = self

  return self
end

-- [[������ ��������]]

function GetData(enemyName, dataName)
    local enemy = _G[enemyName]
    if dataName == "pos_x" then
        return enemy.pos_x
    elseif dataName == "pos_y" then
        return enemy.pos_y
    elseif dataName == "pos_z" then
        return enemy.pos_z
    elseif dataName == "rotate_x" then
        return enemy.rotate_x
    elseif dataName == "rotate_y" then
        return enemy.rotate_y
    elseif dataName == "rotate_z" then
        return enemy.rotate_z
    else
        return nil
    end
end

-- [[�þ߿� �÷��̾ ���Դ��� Ȯ��]]

function ifLookingPlayer(enemyName, px, pz, areaLength)

    local enemy = _G[enemyName]
    local ex = enemy.pos_x
    local ez = enemy.pos_z
    local dir = enemy.dir

    local dx = ex - px
	local dz = ez - pz
	local dist =  math.sqrt(dx * dx + dz * dz)

    -- �÷��̾�� �� ���� ���� ���ϱ�
	local angle = math.atan2(dz, dx) * (180 / PI)
	if angle < 0.0 then
        angle = angle + 360
	elseif angle > 360.0 then
        angle = angle - 360
	end

    -- �ٶ󺸴� ���⿡ ���� �þ� ����(�밢 ��и�) ���̸�
	-- true�� ��ȯ�ϰ� �׷��� ������ false�� ��ȯ

    if dir == "LEFT" then		
        if angle >= 225 and angle < 315 and dist <= areaLength then
			return true 
		else 
            return false
        end
        
    elseif dir == "RIGHT" then		
        if angle >= 45 and angle < 135 and dist <= areaLength then
			return true 
		else 
            return false
        end

    elseif dir == "UP" then		
        if (angle >= 315 or angle < 45) and dist <= areaLength then
			return true 
		else 
            return false
        end

    elseif dir == "DOWN" then		
        if angle >= 135 and angle < 225 and dist <= areaLength then
			return true 
		else 
            return false
        end

    end

end

-- [[�ൿ Ʈ��]]

function BehaviorTree(enemyName, px, pz)
    local enemy = _G[enemyName]

    -- (����) ���� ���� ���� �ִٸ� �÷��̾ �����Ѵ�
    if false then
        enemy.nowState = "ATTACKPLAYER"

    -- (����) �÷��̾ �ٶ󺸰� �ִٸ� (��и� �þ� �ȿ� �ִٸ�)
	-- �÷��̾�� �̵��Ѵ�
    elseif false then
    -- elseif IfLookingPlayer(enemyName, px, pz, LOOKINGAREA) then
        enemy.nowState = "MOVETOPLAYER"

    -- Default (�⺻ ���) ������ �� �������� �����Ѵ�
    else
        enemy.nowState = "DEFAULT"

    end
end

-- [[�� ���� ����]]

-- �ൿġ
local behaviorpoint = 0

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
			enemy.rotate_y = 1.5 * PI
			enemy.pos_x = enemy.pos_x - DEFAULTWALKLENGTH;
            enemy.dir = "LEFT"

        -- �Ʒ�
		elseif behaviorpoint >= 1000 then 
			enemy.rotate_y = 1.0 * PI
			enemy.pos_z = enemy.pos_z - DEFAULTWALKLENGTH;
            enemy.dir = "DOWN"

        -- ������
		elseif behaviorpoint >= 500 then
			enemy.rotate_y = 0.5 * PI
			enemy.pos_x = enemy.pos_x + DEFAULTWALKLENGTH;
            enemy.dir = "RIGHT"

		-- ��
		else
			enemy.rotate_y = 0.0 * PI
			enemy.pos_z = enemy.pos_z + DEFAULTWALKLENGTH;
            enemy.dir = "UP"

        end
    end
end