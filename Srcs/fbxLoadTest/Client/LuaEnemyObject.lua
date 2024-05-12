-- EnemyObject 테이블을 만든다
EnemyObject = {}

-- 상수들 
local PI = 3.141592654 -- 원주율
local LOOKINGAREA = 8.0 -- 적 시야
local ATTACKAREA = 2.2 -- 적 공격 범위
local DIROFFSET = 0.24 -- 적 방향 보정
local DEFAULTWALKLENGTH = 0.005 -- 순찰시 한 번에 이동하는 거리
local FOLLOWWALKLENGTH = 0.012 -- 한 번에 이동하는 거리

-- 새로운 EnemyObject를 생성하여 초기화한다.
function EnemyObject.new(name, hp, x, y, z)
  local self = {
    nowState = "DEFAULT",
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

-- x좌표 가져오기
function GetX(name)
    local enemy = _G[name]
    if enemy then
        return enemy.x
    else
        return nil
    end
end

-- y좌표 가져오기
function GetY(name)
    local enemy = _G[name]
    if enemy then
        return enemy.y
    else
        return nil
    end
end

-- z좌표 가져오기
function GetZ(name)
    local enemy = _G[name]
    if enemy then
        return enemy.z
    else
        return nil
    end
end

-- 행동 트리
function BehaviorTree(name)
    local enemy = _G[name]

    -- (예정) 공격 범위 내에 있다면 플레이어를 공격한다
    if false then
        enemy.nowState = "ATTACKPLAYER"

    -- (예정) 플레이어를 바라보고 있다면 (사분면 시야 안에 있다면)
	-- 플레이어에게 이동한다
    elseif false then
        enemy.nowState = "MOVETOPLAYER"

    -- Default (기본 : 대기) 동작을 맨 마지막에 수행한다
    else
        enemy.nowState = "DEFAULT"

    end
end

-- 행동치
local behaviorpoint = 0

-- 적 동작 수행
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
			-- rotate_y = 1.5 * XM_PI;
			enemy.x = enemy.x - DEFAULTWALKLENGTH;

        -- 아래
		elseif behaviorpoint >= 1000 then 
			-- rotate_y = 1.0 * XM_PI;
			enemy.z = enemy.z - DEFAULTWALKLENGTH;

        -- 오른쪽
		elseif behaviorpoint >= 500 then
			-- rotate_y = 0.5 * XM_PI;
			enemy.x = enemy.x + DEFAULTWALKLENGTH;

		-- 위
		else
			-- rotate_y = 0.0 * XM_PI;
			enemy.z = enemy.z + DEFAULTWALKLENGTH;

        end
    end
end