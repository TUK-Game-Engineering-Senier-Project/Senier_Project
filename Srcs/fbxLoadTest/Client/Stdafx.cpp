#include "Stdafx.h"

// [추가한 내용들]
// bool Object::IfLookingPlayer(char dir, Position enemyPos, Position playerPos)
// void Object::DoAction(char c)
// void Object::BehaviorTree()

constexpr float XM_PI = 3.141592654f; // 원주율

constexpr float LOOKINGAREA = 8.0f; // 적 시야
constexpr float DIROFFSET = 0.24f; // 적 방향 보정
constexpr float DEFAULTWALKLENGTH = 0.005f; // 순찰시 한 번에 이동하는 거리
constexpr float FOLLOWWALKLENGTH = 0.012f; // 한 번에 이동하는 거리

// 시야에 플레이어가 들어왔는지 확인
bool Object::IfLookingPlayer(char dir, Position enemyPos, Position playerPos)
{
	float px = playerPos.x;
	float pz = playerPos.z;
	float ex = enemyPos.x;
	float ez = enemyPos.z;

	// 플레이어와 적 사이 거리 구하기
	float fDx = ex - px;
	float fDz = ez - pz;
	float fDist = sqrt(fDx * fDx + fDz * fDz);

	// 플레이어와 적 사이 각도 구하기
	float angle = atan2(fDx, fDz) * (180.0f / XM_PI);
	if (angle < 0.0f) { angle += 360.0f; }
	if (angle > 360.0f) { angle -= 360.0f; }

	// 바라보는 방향에 따라 시야 범위(대각 사분면) 내이면
	// true를 반환하고 그렇지 않으면 false를 반환

	switch (dir)
	{
	case OBJECT_DIR::LEFT:

		if ((angle >= 225.0f && angle < 315.0f) && (fDist <= LOOKINGAREA)) {
			return true; 
		}
		else { return false; }

		break;

	case OBJECT_DIR::RIGHT:

		if ((angle >= 45.0f && angle < 135.0f) && (fDist <= LOOKINGAREA)) {
			return true;
		}
		else { return false; }

		break;

	case OBJECT_DIR::UP:

		if ((angle >= 315.0f || angle < 45.0f) && (fDist <= LOOKINGAREA)) {
			return true;
		}
		else { return false; }

		break;

	case OBJECT_DIR::DOWN:

		if ((angle >= 135.0f && angle < 225.0f) && (fDist <= LOOKINGAREA)) {
			return true;
		}
		else { return false; }

		break;
	}
}

// 오브젝트 동작 수행
void Object::DoAction(char c)
{
	// 플레이어 위치 / 적 위치
	Position playerPos = Position(player[0].pos_x, player[0].pos_y, player[0].pos_z);
	Position enemyPos = Position(pos_x, pos_y, pos_z);

	float dx = playerPos.x - enemyPos.x;
	float dz = playerPos.z - enemyPos.z;
	float fDist = 0.0f;

	// 현재 동작에 따른 동작 수행
	switch (c) 
	{

	case ACTION::DEFAULT:
		// 기본 동작 (### 임시)
		// # 위->오른쪽->아래->왼쪽 으로 순찰(?) 도는 동작

		// 행동치와 행동치에 따른 동작
		if (behaviorpoint < 2000) behaviorpoint++;
		else behaviorpoint = 0;
		switch (behaviorpoint) {
		case 0: cNowLookingDir = OBJECT_DIR::UP; break;
		case 500: cNowLookingDir = OBJECT_DIR::RIGHT; break;
		case 1000: cNowLookingDir = OBJECT_DIR::DOWN; break;
		case 1500: cNowLookingDir = OBJECT_DIR::LEFT; break;
		}

		// 바라보는 방향에 따라 동작 수행
		switch (cNowLookingDir)
		{
		case OBJECT_DIR::LEFT:
			rotate_y = 1.5 * XM_PI;
			pos_x -= DEFAULTWALKLENGTH;
			break;

		case OBJECT_DIR::RIGHT:
			rotate_y = 0.5 * XM_PI;
			pos_x += DEFAULTWALKLENGTH;
			break;

		case OBJECT_DIR::UP:
			rotate_y = 0.0 * XM_PI;
			pos_z += DEFAULTWALKLENGTH;
			break;

		case OBJECT_DIR::DOWN:
			rotate_y = 1.0 * XM_PI;
			pos_z -= DEFAULTWALKLENGTH;
			break;
		}

		break;

	case ACTION::MOVETOPLAYER:

		// 적이 플레이어를 향해 (8방향) 바라보도록 설정
		if (dx > 0.0f) {
			if (dz > 0.0f) { rotate_y = 0.25f * XM_PI; }
			else if (dz < 0.0f) { rotate_y = 0.75f * XM_PI; }
			else { rotate_y = 0.5f * XM_PI; }
		}
		else if (dx < 0.0f) {
			if (dz > 0.0f) { rotate_y = 1.75f * XM_PI; }
			else if (dz < 0.0f) { rotate_y = 1.25f * XM_PI; }
			else { rotate_y = 1.5f * XM_PI; }
		}
		else {
			if (dz >= 0.0f) { rotate_y = 1.0f * XM_PI; }
			else { rotate_y = 0.0f * XM_PI; }
		}

		// 적 이동 거리 지정
		fDist = sqrt(dx * dx + dz * dz);
		if (fDist != 0.0f) {
			dx /= fDist;
			dz /= fDist;
		}
		pos_x += dx * FOLLOWWALKLENGTH;
		pos_z += dz * FOLLOWWALKLENGTH;

		// 위치 보정
		if ((pos_x >= playerPos.x - DIROFFSET) && (pos_x <= playerPos.x + DIROFFSET)) 
		{ pos_x = playerPos.x; }
		if ((pos_z >= playerPos.z - DIROFFSET) && (pos_z <= playerPos.z + DIROFFSET)) 
		{ pos_z = playerPos.z; }

		break;

	case ACTION::ATTACKPLAYER:
		// (예정) 플레이어 공격
		break;

	}
}

// 행동 트리
void Object::BehaviorTree()
{
	// 플레이어 위치 / 적 위치
	Position PlayerPos = Position(player[0].pos_x, player[0].pos_y, player[0].pos_z);
	Position enemyPos = Position(pos_x, pos_y, pos_z);

	// 플레이어를 바라보고 있다면 (사분면 시야 안에 있다면)
	// 플레이어에게 이동한다.
	if (IfLookingPlayer(cNowLookingDir, PlayerPos, enemyPos)) {
		cNowAction = ACTION::MOVETOPLAYER;
	}

	// (예정) 조건을 만족하면 플레이어를 공격한다.
	else if (false) {
		cNowAction = ACTION::ATTACKPLAYER;
	}

	// Default 동작을 맨 마지막에 수행한다.
	else {
		cNowAction = ACTION::DEFAULT;
	}
}
