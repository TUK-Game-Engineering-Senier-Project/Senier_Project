#pragma once

// <���� ������Ʈ ��� ����>

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
};