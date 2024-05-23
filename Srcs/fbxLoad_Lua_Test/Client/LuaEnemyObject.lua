-- [[Lua로 작성한 Object 관련 내용 코드]]

-- 상수들 
local PI = 3.141592654 -- 원주율
local LOOKINGAREA = 6.0 -- 적 시야
local ATTACKAREA = 2.2 -- 적 공격 범위
local DIROFFSET = 0.24 -- 적 방향 보정
local DEFAULTWALKLENGTH = 0.005 -- 순찰시 한 번에 이동하는 거리
local FOLLOWWALKLENGTH = 0.012 -- 플레이어 추격시 이동하는 거리

-- Object 테이블을 만든다
EnemyObject = {}
PlayerObject = {}

-- ### 플레이어 오브젝트도 lua로 가져올 예정
-- ### Stdafx.h의 Object 구조체 지우기
-- ### LuaEnemyObject.h에서 Position 구조체 지우기
-- ### PlayerObject도 EnemyObject.new처럼 초기화할 것

-- [[플레이어 위치]]

local player_x = 0.0
local player_y = 0.0
local player_z = 0.0

-- 위치 가져오기
function GetPlayerPosition()
    return player_x, player_y, player_z
end

-- 위치 업데이트하기
function UpdatePlayerPosition(x, y, z)
    player_x = x
    player_y = y 
    player_z = z 
end

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

function IfLookingPlayer(enemyName, px, pz, areaLength)

    local enemy = _G[enemyName]

    -- 사용할 지역변수들
    local ex = enemy.pos_x -- 적 x좌표
    local ez = enemy.pos_z -- 적 z좌표
    local dx = px - ex -- x좌표 차이
	local dz = pz - ez -- z좌표 차이
	local dist = math.sqrt(dx * dx + dz * dz) -- 적과 플레이어 사이 거리

    -- 플레이어와 적 사이 각도 구하기
	local angle = math.atan2(dz, dx) * (180 / PI)
	if angle < 0.0 then
        angle = angle + 360
	elseif angle > 360.0 then
        angle = angle - 360
	end

    -- 바라보는 방향에 따라 시야 범위(대각 사분면) 내이면
	-- true를 반환하고 그렇지 않으면 false를 반환

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

-- [[행동 트리]]

function BehaviorTree(enemyName, px, pz)
    local enemy = _G[enemyName]

    -- 플레이어 위치 갱신
    UpdatePlayerPosition(px, 0.0, pz)

    -- (공격 범위 내에 있다면 플레이어를 공격한다
    if IfLookingPlayer(enemyName, px, pz, ATTACKAREA) then
        enemy.nowState = "ATTACKPLAYER"

    -- 플레이어를 바라보고 있다면 (사분면 시야 안에 있다면)
	-- 플레이어에게 이동한다
    elseif IfLookingPlayer(enemyName, px, pz, LOOKINGAREA) then
        enemy.nowState = "MOVETOPLAYER"

    -- Default (기본 대기) 동작을 맨 마지막에 수행한다
    else
        enemy.nowState = "DEFAULT"

    end
end

-- [[적 동작 수행]]

-- 행동치
local behaviorpoint = 0

function DoAction(name, px, pz)
    local enemy = _G[name]

    -- 사용할 지역변수들
    local ex = enemy.pos_x -- 적 x좌표
    local ez = enemy.pos_z -- 적 z좌표
    local dir = enemy.dir -- 적 방향
    local dx = px - ex -- x좌표 차이
	local dz = pz - ez -- z좌표 차이
	local dist = math.sqrt(dx * dx + dz * dz) -- 적과 플레이어 사이 거리
	 
    -- 현재 동작에 따른 동작 수행

    if enemy.nowState == "ATTACKPLAYER" then
        
        -- 행동치와 행동치에 따른 동작

		if behaviorpoint < 70 then
            behaviorpoint = behaviorpoint + 1
		else
            behaviorpoint = 0
        end

		if behaviorpoint < 50 then
			-- 회전 모션 (선딜레이?)
			enemy.rotate_y = enemy.rotate_y - (2.0 * PI / 50)
		elseif behaviorpoint < 70 then
			-- 후 공격 (플레이어 밀쳐내기)
			player_x = px + dx * 2
			player_z = pz + dz * 2
			behaviorpoint = 0
        end

    elseif enemy.nowState == "MOVETOPLAYER" then
        
        -- 적이 플레이어를 향해 (8방향) 바라보도록 설정

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

        -- 적 이동 거리 지정
		if dist ~= 0.0 then
			dx = dx / dist
			dz = dz / dist
		end
		enemy.pos_x = enemy.pos_x + dx * FOLLOWWALKLENGTH
		enemy.pos_z = enemy.pos_z + dz * FOLLOWWALKLENGTH

        -- 위치 보정
        if enemy.pos_x >= px - DIROFFSET and enemy.pos_x <= px + DIROFFSET then
            enemy.pos_x = px
        end 
        if enemy.pos_z >= pz - DIROFFSET and enemy.pos_z <= pz + DIROFFSET then
            enemy.pos_z = pz
        end

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



-- ### 테스트용 [출력] 데이터 표시 장치
-- ### 사용 완료 후 지울 것
-- LuaDebugOutput("MOVETOPLAYER\n")