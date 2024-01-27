#pragma once

// <게임 오브젝트 헤더 파일>

class g_cPlayer 
{
public:
    // 플레이어 종류 (master(방장) / visitor(손님))
    char cPlayerType; 

    // 각 키 입력여부
    bool 
        bForwardKeyDown{ false }, 
        bBackKeyDown{ false }, 
        bLeftKeyDown{ false }, 
        bRightKeyDown{ false };
};