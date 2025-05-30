#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"
#include <d3d12.h>



//#include "HealthBar.h"
#include "d3dx12.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

struct PlayerPreset
{
    // 방어 관련
    int hp = 500;
    int hpMax = -1;              // 초기값 -1
    int shield_type = 0;         // 0: 없음, 1: 중형, 2: 대형
    int block_strength = 50;
    int block_speed = 50;
    int dodge_decision = 50;
    int dodge_speed = 50;
    bool can_jump = true;

    // 공격 관련
    int weapon_type = 0;         // 0: 소형, 1: 중형, 2: 대형
    int damage = 50;
    int attack_speed = 50;
    int move_speed = 50;
    int run_speed = 50;

    // 기타
    bool key_merge = true;
};


class UIManager {
public:
    UIManager();
    ~UIManager();

    bool Initialize(HWND hwnd, ID3D12Device* device, ID3D12DescriptorHeap* srvDescHeap);
    void Shutdown();
    void NewFrame();
    void Render(ID3D12GraphicsCommandList* commandList);

    void MainMenuUI();
    void SingleSettingUI();
    void SinglePlayUI();
    void MultiSettingUI();
    void MultiPlayUI();

    PlayerPreset player;

    // 체력바 관리 함수
    void DecreasePlayerHp();
    void PrepareHpReset();  // 체력 최대값을 초기화 준비 상태로 설정

private:
    bool m_Initialized;
    ID3D12Device* m_Device;
    ID3D12DescriptorHeap* m_SrvDescHeap;

   
  
};