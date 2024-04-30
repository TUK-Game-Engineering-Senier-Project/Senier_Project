#include "Stdafx.h"

// void Object::DoAction(char c) : 오브젝트 동작 수행
// void Object::BehaviorTree()   : 오브젝트 행동 트리

constexpr float XM_PI = 3.141592654f;

// 오브젝트 동작 수행
void Object::DoAction(char c)
{
	// 현재 동작에 따른 동작 수행
	switch (c) 
	{

	case ACTION::DEFAULT:
		// 기본 동작 (### 임시)
		// # 위->오른쪽->아래->왼쪽 으로 순찰(?) 도는 동작

		// 행동치와 행동치에 따른 동작
		if (behaviorpoint < 1600) behaviorpoint++;
		else behaviorpoint = 0;
		switch (behaviorpoint) {
		case 0: nowLookingDir = OBJECT_DIR::UP; break;
		case 400: nowLookingDir = OBJECT_DIR::RIGHT; break;
		case 800: nowLookingDir = OBJECT_DIR::DOWN; break;
		case 1200: nowLookingDir = OBJECT_DIR::LEFT; break;
		}

		// 바라보는 방향에 따라 동작 수행
		switch (nowLookingDir)
		{
		case OBJECT_DIR::LEFT:
			rotate_y = 1.5 * XM_PI;
			pos_x -= 0.01;
			break;
		case OBJECT_DIR::RIGHT:
			rotate_y = 0.5 * XM_PI;
			pos_x += 0.01;
			break;
		case OBJECT_DIR::UP:
			rotate_y = 0.0 * XM_PI;
			pos_z += 0.01;
			break;
		case OBJECT_DIR::DOWN:
			rotate_y = 1.0 * XM_PI;
			pos_z -= 0.01;
			break;
		}

		break;

	case ACTION::MOVETOPLAYER:
		// (예정) 플레이어에게 이동
		break;

	case ACTION::ATTACKPLAYER:
		// (예정) 플레이어 공격
		break;

	}
}

// 행동 트리
void Object::BehaviorTree()
{
	// (예정) 조건을 만족하면 플레이어에게 이동한다.
	if (false) {
		cNowAction = ACTION::MOVETOPLAYER;
	}

	// (예정) 조건을 만족하면 플레이어를 공격한다.
	if (false) {
		cNowAction = ACTION::ATTACKPLAYER;
	}

	// Default 동작을 맨 마지막에 수행한다.
	else {
		cNowAction = ACTION::DEFAULT;
	}
}
