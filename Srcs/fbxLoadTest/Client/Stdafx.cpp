#include "Stdafx.h"

// void Object::DoAction(char c) : ������Ʈ ���� ����
// void Object::BehaviorTree()   : ������Ʈ �ൿ Ʈ��

constexpr float XM_PI = 3.141592654f;

// ������Ʈ ���� ����
void Object::DoAction(char c)
{
	// ���� ���ۿ� ���� ���� ����
	switch (c) 
	{

	case ACTION::DEFAULT:
		// �⺻ ���� (### �ӽ�)
		// # ��->������->�Ʒ�->���� ���� ����(?) ���� ����

		// �ൿġ�� �ൿġ�� ���� ����
		if (behaviorpoint < 1600) behaviorpoint++;
		else behaviorpoint = 0;
		switch (behaviorpoint) {
		case 0: nowLookingDir = OBJECT_DIR::UP; break;
		case 400: nowLookingDir = OBJECT_DIR::RIGHT; break;
		case 800: nowLookingDir = OBJECT_DIR::DOWN; break;
		case 1200: nowLookingDir = OBJECT_DIR::LEFT; break;
		}

		// �ٶ󺸴� ���⿡ ���� ���� ����
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
		// (����) �÷��̾�� �̵�
		break;

	case ACTION::ATTACKPLAYER:
		// (����) �÷��̾� ����
		break;

	}
}

// �ൿ Ʈ��
void Object::BehaviorTree()
{
	// (����) ������ �����ϸ� �÷��̾�� �̵��Ѵ�.
	if (false) {
		cNowAction = ACTION::MOVETOPLAYER;
	}

	// (����) ������ �����ϸ� �÷��̾ �����Ѵ�.
	if (false) {
		cNowAction = ACTION::ATTACKPLAYER;
	}

	// Default ������ �� �������� �����Ѵ�.
	else {
		cNowAction = ACTION::DEFAULT;
	}
}
