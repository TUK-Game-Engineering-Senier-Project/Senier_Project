-- [[Lua�� �ۼ��� Object ���� ���� �ڵ�]]

-- ����� 
local PI = 3.141592654 -- ������
local LOOKINGAREA = 6.0 -- �� �þ�
local ATTACKAREA = 2.2 -- �� ���� ����
local DIROFFSET = 0.24 -- �� ���� ����
local DEFAULTWALKLENGTH = 0.005 -- ������ �� ���� �̵��ϴ� �Ÿ�
local FOLLOWWALKLENGTH = 0.012 -- �÷��̾� �߰ݽ� �̵��ϴ� �Ÿ�

-- Object ���̺��� �����
EnemyObject = {}
PlayerObject = {}

-- ### �÷��̾� ������Ʈ�� lua�� ������ ����
-- ### Stdafx.h�� Object ����ü �����
-- ### LuaEnemyObject.h���� Position ����ü �����
-- ### PlayerObject�� EnemyObject.newó�� �ʱ�ȭ�� ��

-- [[�÷��̾� ��ġ]]

local player_x = 0.0
local player_y = 0.0
local player_z = 0.0

-- ��ġ ��������
function GetPlayerPosition()
    return player_x, player_y, player_z
end

-- ��ġ ������Ʈ�ϱ�
function UpdatePlayerPosition(x, y, z)
    player_x = x
    player_y = y 
    player_z = z 
end

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

function IfLookingPlayer(enemyName, px, pz, areaLength)

    local enemy = _G[enemyName]

    -- ����� ����������
    local ex = enemy.pos_x -- �� x��ǥ
    local ez = enemy.pos_z -- �� z��ǥ
    local dx = px - ex -- x��ǥ ����
	local dz = pz - ez -- z��ǥ ����
	local dist = math.sqrt(dx * dx + dz * dz) -- ���� �÷��̾� ���� �Ÿ�

    -- �÷��̾�� �� ���� ���� ���ϱ�
	local angle = math.atan2(dz, dx) * (180 / PI)
	if angle < 0.0 then
        angle = angle + 360
	elseif angle > 360.0 then
        angle = angle - 360
	end

    -- �ٶ󺸴� ���⿡ ���� �þ� ����(�밢 ��и�) ���̸�
	-- true�� ��ȯ�ϰ� �׷��� ������ false�� ��ȯ

    if enemy.dir == "LEFT" then		
        if angle >= 135 and angle < 225 and dist <= areaLength then
			return true 
		else 
            return false
        end
        
    elseif enemy.dir == "RIGHT" then		
        if angle >= 315 and dist <= areaLength then
			return true 
        elseif angle < 45 and dist <= areaLength then
			return true
		else 
            return false
        end

    elseif enemy.dir == "DOWN" then		
        if angle >= 225 and angle < 315 and dist <= areaLength then
			return true 
		else 
            return false
        end

    elseif enemy.dir == "UP" then		
        if angle >= 45 and angle < 135 and dist <= areaLength then
			return true 
		else 
            return false
        end

    end

end

-- [[�ൿ Ʈ��]]

function BehaviorTree(enemyName, px, pz)
    local enemy = _G[enemyName]

    -- �÷��̾� ��ġ ����
    UpdatePlayerPosition(px, 0.0, pz)

    -- (���� ���� ���� �ִٸ� �÷��̾ �����Ѵ�
    if IfLookingPlayer(enemyName, px, pz, ATTACKAREA) then
        enemy.nowState = "ATTACKPLAYER"

    -- �÷��̾ �ٶ󺸰� �ִٸ� (��и� �þ� �ȿ� �ִٸ�)
	-- �÷��̾�� �̵��Ѵ�
    elseif IfLookingPlayer(enemyName, px, pz, LOOKINGAREA) then
        enemy.nowState = "MOVETOPLAYER"

    -- Default (�⺻ ���) ������ �� �������� �����Ѵ�
    else
        enemy.nowState = "DEFAULT"

    end
end

-- [[�� ���� ����]]

-- �ൿġ
local behaviorpoint = 0

function DoAction(name, px, pz)
    local enemy = _G[name]

    -- ����� ����������
    local ex = enemy.pos_x -- �� x��ǥ
    local ez = enemy.pos_z -- �� z��ǥ
    local dir = enemy.dir -- �� ����
    local dx = px - ex -- x��ǥ ����
	local dz = pz - ez -- z��ǥ ����
	local dist = math.sqrt(dx * dx + dz * dz) -- ���� �÷��̾� ���� �Ÿ�
	 
    -- ���� ���ۿ� ���� ���� ����

    if enemy.nowState == "ATTACKPLAYER" then
        
        -- �ൿġ�� �ൿġ�� ���� ����

		if behaviorpoint < 70 then
            behaviorpoint = behaviorpoint + 1
		else
            behaviorpoint = 0
        end

		if behaviorpoint < 50 then
			-- ȸ�� ��� (��������?)
			enemy.rotate_y = enemy.rotate_y - (2.0 * PI / 50)
		elseif behaviorpoint < 70 then
			-- �� ���� (�÷��̾� ���ĳ���)
			player_x = px + dx * 2
			player_z = pz + dz * 2
			behaviorpoint = 0
        end

    elseif enemy.nowState == "MOVETOPLAYER" then
        
        -- ���� �÷��̾ ���� (8����) �ٶ󺸵��� ����

        if dx > 0.0 then
            if dz > 0.0 then
                enemy.rotate_y = 0.25 * PI
            elseif dz < 0.0 then
                enemy.rotate_y = 0.75 * PI
            else
                enemy.rotate_y = 0.5 * PI
            end

        elseif dx < 0.0 then
            if dz > 0.0 then
                enemy.rotate_y = 1.75 * PI
            elseif dz < 0.0 then
                enemy.rotate_y = 1.25 * PI
            else
                enemy.rotate_y = 1.5 * PI
            end

        else
            if dz > 0.0 then
                enemy.rotate_y = 1.0 * PI
            else
                enemy.rotate_y = 0.0 * PI
            end

        end

        -- �� �̵� �Ÿ� ����
		if dist ~= 0.0 then
			dx = dx / dist
			dz = dz / dist
		end
		enemy.pos_x = enemy.pos_x + dx * FOLLOWWALKLENGTH
		enemy.pos_z = enemy.pos_z + dz * FOLLOWWALKLENGTH

        -- ��ġ ����
        if enemy.pos_x >= px - DIROFFSET and enemy.pos_x <= px + DIROFFSET then
            enemy.pos_x = px
        end 
        if enemy.pos_z >= pz - DIROFFSET and enemy.pos_z <= pz + DIROFFSET then
            enemy.pos_z = pz
        end

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



-- ### �׽�Ʈ�� [���] ������ ǥ�� ��ġ
-- ### ��� �Ϸ� �� ���� ��
-- LuaDebugOutput("MOVETOPLAYER\n")