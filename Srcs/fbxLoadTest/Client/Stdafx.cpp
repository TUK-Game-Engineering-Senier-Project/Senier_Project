#include "Stdafx.h"

// [�߰��� �����]
// bool Object::IfLookingPlayer(char dir, Position enemyPos, Position playerPos)
// void Object::DoAction(char c)
// void Object::BehaviorTree()

constexpr float XM_PI = 3.141592654f; // ������

constexpr float LOOKINGAREA = 8.0f; // �� �þ�
constexpr float DIROFFSET = 0.24f; // �� ���� ����
constexpr float DEFAULTWALKLENGTH = 0.005f; // ������ �� ���� �̵��ϴ� �Ÿ�
constexpr float FOLLOWWALKLENGTH = 0.012f; // �� ���� �̵��ϴ� �Ÿ�

// �þ߿� �÷��̾ ���Դ��� Ȯ��
bool Object::IfLookingPlayer(char dir, Position enemyPos, Position playerPos)
{
	float px = playerPos.x;
	float pz = playerPos.z;
	float ex = enemyPos.x;
	float ez = enemyPos.z;

	// �÷��̾�� �� ���� �Ÿ� ���ϱ�
	float fDx = ex - px;
	float fDz = ez - pz;
	float fDist = sqrt(fDx * fDx + fDz * fDz);

	// �÷��̾�� �� ���� ���� ���ϱ�
	float angle = atan2(fDx, fDz) * (180.0f / XM_PI);
	if (angle < 0.0f) { angle += 360.0f; }
	if (angle > 360.0f) { angle -= 360.0f; }

	// �ٶ󺸴� ���⿡ ���� �þ� ����(�밢 ��и�) ���̸�
	// true�� ��ȯ�ϰ� �׷��� ������ false�� ��ȯ

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

// ������Ʈ ���� ����
void Object::DoAction(char c)
{
	// �÷��̾� ��ġ / �� ��ġ
	Position playerPos = Position(player[0].pos_x, player[0].pos_y, player[0].pos_z);
	Position enemyPos = Position(pos_x, pos_y, pos_z);

	float dx = playerPos.x - enemyPos.x;
	float dz = playerPos.z - enemyPos.z;
	float fDist = 0.0f;

	// ���� ���ۿ� ���� ���� ����
	switch (c) 
	{

	case ACTION::DEFAULT:
		// �⺻ ���� (### �ӽ�)
		// # ��->������->�Ʒ�->���� ���� ����(?) ���� ����

		// �ൿġ�� �ൿġ�� ���� ����
		if (behaviorpoint < 2000) behaviorpoint++;
		else behaviorpoint = 0;
		switch (behaviorpoint) {
		case 0: cNowLookingDir = OBJECT_DIR::UP; break;
		case 500: cNowLookingDir = OBJECT_DIR::RIGHT; break;
		case 1000: cNowLookingDir = OBJECT_DIR::DOWN; break;
		case 1500: cNowLookingDir = OBJECT_DIR::LEFT; break;
		}

		// �ٶ󺸴� ���⿡ ���� ���� ����
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

		// ���� �÷��̾ ���� (8����) �ٶ󺸵��� ����
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

		// �� �̵� �Ÿ� ����
		fDist = sqrt(dx * dx + dz * dz);
		if (fDist != 0.0f) {
			dx /= fDist;
			dz /= fDist;
		}
		pos_x += dx * FOLLOWWALKLENGTH;
		pos_z += dz * FOLLOWWALKLENGTH;

		// ��ġ ����
		if ((pos_x >= playerPos.x - DIROFFSET) && (pos_x <= playerPos.x + DIROFFSET)) 
		{ pos_x = playerPos.x; }
		if ((pos_z >= playerPos.z - DIROFFSET) && (pos_z <= playerPos.z + DIROFFSET)) 
		{ pos_z = playerPos.z; }

		break;

	case ACTION::ATTACKPLAYER:
		// (����) �÷��̾� ����
		break;

	}
}

// �ൿ Ʈ��
void Object::BehaviorTree()
{
	// �÷��̾� ��ġ / �� ��ġ
	Position PlayerPos = Position(player[0].pos_x, player[0].pos_y, player[0].pos_z);
	Position enemyPos = Position(pos_x, pos_y, pos_z);

	// �÷��̾ �ٶ󺸰� �ִٸ� (��и� �þ� �ȿ� �ִٸ�)
	// �÷��̾�� �̵��Ѵ�.
	if (IfLookingPlayer(cNowLookingDir, PlayerPos, enemyPos)) {
		cNowAction = ACTION::MOVETOPLAYER;
	}

	// (����) ������ �����ϸ� �÷��̾ �����Ѵ�.
	else if (false) {
		cNowAction = ACTION::ATTACKPLAYER;
	}

	// Default ������ �� �������� �����Ѵ�.
	else {
		cNowAction = ACTION::DEFAULT;
	}
}
