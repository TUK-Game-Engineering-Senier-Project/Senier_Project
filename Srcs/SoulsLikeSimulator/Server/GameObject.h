#pragma once

// <게임 오브젝트 헤더 파일>

// 플레이어 클래스
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
        bRightKeyDown{ false },

        bZDown{ false },
        
        bF1Down{ false },
        bF2Down{ false },
        bF3Down{ false };
    
    float fPos[3] = { 0.0f, 0.0f, 0.0f };    // 위치
    float fRotate[3] = { 0.0f, 0.0f, 0.0f }; // 회전
    float fMove[3] = { 0.0f, 0.0f, 0.0f };   // 이동

    // 점프중 여부
    bool bJumping = false; 
    float fJumpSpd = 0.0f;

    float fRadius = 10.0f;

    // 
    // float fScale[3];  // 크기
    // float fColor[3];  // 색상
};

