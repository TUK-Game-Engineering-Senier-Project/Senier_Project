#pragma once

// <���� ������Ʈ ��� ����>

// �÷��̾� Ŭ����
class g_cPlayer 
{
public:
    // �÷��̾� ���� (master(����) / visitor(�մ�))
    char cPlayerType; 

    // �� Ű �Է¿���
    bool 
        bForwardKeyDown{ false }, 
        bBackKeyDown{ false }, 
        bLeftKeyDown{ false }, 
        bRightKeyDown{ false },

        bZDown{ false };

    // ��ġ
    float fPos[3] = { 0.0f, 0.0f, 0.0f }; 

    // ������ ����
    bool bJumping = false; 
    float fJumpSpd = 0.0f;

    // float fRotate[3]; // ȸ��
    // float fScale[3];  // ũ��
    // float fColor[3];  // ����
};

