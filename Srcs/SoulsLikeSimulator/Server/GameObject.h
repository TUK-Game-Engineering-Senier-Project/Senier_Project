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

        bZDown{ false },
        
        bF1Down{ false },
        bF2Down{ false },
        bF3Down{ false };
    
    float fPos[3] = { 0.0f, 0.0f, 0.0f };    // ��ġ
    float fRotate[3] = { 0.0f, 0.0f, 0.0f }; // ȸ��

    // ������ ����
    bool bJumping = false; 
    float fJumpSpd = 0.0f;

    float fRadius = 10.0f;
    
    char cNowAction = 100; // ���� ���� (ACTION_NONE)

    // float fScale[3];  // ũ��
    // float fColor[3];  // ����
};

