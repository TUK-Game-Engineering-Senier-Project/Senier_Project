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
        bRightKeyDown{ false };

    float fPos[3] = { 0.0f, 0.0f, 0.0f };    // ��ġ
    float fRotate[3]; // ȸ��
    float fScale[3];  // ũ��
    float fColor[3];  // ����
};

