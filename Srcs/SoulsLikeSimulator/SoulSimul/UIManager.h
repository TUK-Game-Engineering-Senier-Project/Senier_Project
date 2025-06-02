#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"
#include <d3d12.h>

#include <string>
#include <vector>

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
    int potionCount = 5;
    int stamina = 100;
    int staminaMax = 100;
};

struct PatternData {
    std::string name;
    float cooldown = 3.0f;
    int damage = 50;
    bool is_parryable = true;
    bool has_knockback = false;
    // 확장 가능: animationName, rangeType 등
};

struct MonsterPreset
{
    // 기본 능력치
    int hp = 1000;
    int hpMax = -1;

    // 크기 타입: 0 = 소형, 1 = 중형, 2 = 대형
    int size_type = 0;

    std::vector<PatternData> patterns;

    // 패턴 토글 항목
    bool use_slash = true;           // 일반 베기
    bool use_quick_slash = false;    // 빠른 베기
    bool use_stab = false;           // 찌르기
    bool use_heavy_slash = false;    // 강한 베기
    bool use_dash_slash = false;     // 돌진 베기
    bool use_parry_chance = false;   // 확률 패링
    bool use_dodge_chance = false;   // 확률 회피


    // 공격 관련
    int damage = 80;
    int attack_speed = 40;
    int pattern_count = 3;       // 공격 패턴 수 (단일 or 페이즈 기반)

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
    void SingleSettingUI();     //유저
    void SingleSettingUI2();    //보스
    void SinglePlayUI();
    void MultiSettingUI();
    void MultiPlayUI();

    PlayerPreset player;
    MonsterPreset monster;

    // 체력바 관리 함수
    void DecreasePlayerHp();
    void DecreaseMonsterHp();
    void PrepareHpReset();  // 체력 최대값을 초기화 준비 상태로 설정
    void UsePotion();
    void DecreaseStamina();

    void PrepareSceneData(); // 신 전환시 초기화

private:
    bool m_Initialized;
    ID3D12Device* m_Device;
    ID3D12DescriptorHeap* m_SrvDescHeap;

   
  
};