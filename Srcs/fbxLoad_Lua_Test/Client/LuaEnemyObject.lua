-- EnemyObject 테이블을 만든다
EnemyObject = {}

-- 상수들 
local PI = 3.141592654 -- 원주율
local LOOKINGAREA = 8.0 -- 적 시야
local ATTACKAREA = 2.2 -- 적 공격 범위
local DIROFFSET = 0.24 -- 적 방향 보정
local DEFAULTWALKLENGTH = 0.005 -- 순찰시 한 번에 이동하는 거리
local FOLLOWWALKLENGTH = 0.012 -- 한 번에 이동하는 거리

-- [[새로운 EnemyObject를 생성하여 초기화]]

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

  -- 해당 EnemyObject의 name을 가지고
  -- EnemyObject를 전역 오브젝트로 저장한다
  _G[name] = self

  return self
end

-- [[데이터 가져오기]]

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

-- [[시야에 플레이어가 들어왔는지 확인]]

function ifLookingPlayer(enemyName, px, pz, areaLength)

    local enemy = _G[enemyName]
    local ex = enemy.pos_x
    local ez = enemy.pos_z
    local dir = enemy.dir

    local dx = ex - px
	local dz = ez - pz
	local dist =  math.sqrt(dx * dx + dz * dz)

    -- 플레이어와 적 사이 각도 구하기
	local angle = math.atan2(dz, dx) * (180 / PI)
	if angle < 0.0 then
        angle = angle + 360
	elseif angle > 360.0 then
        angle = angle - 360
	end

    -- 바라보는 방향에 따라 시야 범위(대각 사분면) 내이면
	-- true를 반환하고 그렇지 않으면 false를 반환

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

-- [[행동 트리]]

function BehaviorTree(enemyName, px, pz)
    local enemy = _G[enemyName]

    -- (예정) 공격 범위 내에 있다면 플레이어를 공격한다
    if false then
        enemy.nowState = "ATTACKPLAYER"

    -- (예정) 플레이어를 바라보고 있다면 (사분면 시야 안에 있다면)
	-- 플레이어에게 이동한다
    elseif false then
    -- elseif IfLookingPlayer(enemyName, px, pz, LOOKINGAREA) then
        enemy.nowState = "MOVETOPLAYER"

    -- Default (기본 대기) 동작을 맨 마지막에 수행한다
    else
        enemy.nowState = "DEFAULT"

    end
end

-- [[적 동작 수행]]

-- 행동치
local behaviorpoint = 0

function DoAction(name)
    local enemy = _G[name]
	 
    -- 현재 동작에 따른 동작 수행

    if enemy.nowState == "ATTACKPLAYER" then
        enemy.nowState = "ATTACKPLAYER"

    elseif enemy.nowState == "MOVETOPLAYER" then
        enemy.nowState = "MOVETOPLAYER"

    elseif enemy.nowState == "DEFAULT" then
        
        -- 기본 (대기) 동작 (### 임시)
		-- 위->오른쪽->아래->왼쪽 으로 순찰(?) 도는 동작

		-- 행동치와 행동치에 따른 동작
		if behaviorpoint < 2000 then 
            behaviorpoint = behaviorpoint + 1
		else 
            behaviorpoint = 0
        end

        -- 왼쪽
		if behaviorpoint >= 1500 then 
			enemy.rotate_y = 1.5 * PI
			enemy.pos_x = enemy.pos_x - DEFAULTWALKLENGTH;
            enemy.dir = "LEFT"

        -- 아래
		elseif behaviorpoint >= 1000 then 
			enemy.rotate_y = 1.0 * PI
			enemy.pos_z = enemy.pos_z - DEFAULTWALKLENGTH;
            enemy.dir = "DOWN"

        -- 오른쪽
		elseif behaviorpoint >= 500 then
			enemy.rotate_y = 0.5 * PI
			enemy.pos_x = enemy.pos_x + DEFAULTWALKLENGTH;
            enemy.dir = "RIGHT"

		-- 위
		else
			enemy.rotate_y = 0.0 * PI
			enemy.pos_z = enemy.pos_z + DEFAULTWALKLENGTH;
            enemy.dir = "UP"

        end
    end
end