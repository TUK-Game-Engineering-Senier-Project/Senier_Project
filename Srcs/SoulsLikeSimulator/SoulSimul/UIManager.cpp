 #include "UIManager.h"
#include "define.h"
#include <DirectXMath.h>  // 추가
using namespace DirectX;   // 추가

#define U8(text) reinterpret_cast<const char*>(u8##text) // 한글 사용 

UIManager::UIManager() {}

UIManager::~UIManager() {
    Shutdown();
}

bool UIManager::Initialize(HWND hwnd, ID3D12Device* device, ID3D12DescriptorHeap* srvDescHeap) {
    m_Device = device;
    m_SrvDescHeap = srvDescHeap;

    // ImGui 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // 기본 ImGui 설정
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 키보드 지원
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // 게임패드 지원
    ImGui::StyleColorsDark(); // 다크 모드 스타일

    const char* fontPath = "C:/Windows/Fonts/malgun.ttf"; // 경로 수정
    float fontSize = 16.0f;

    ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath, fontSize, NULL, io.Fonts->GetGlyphRangesKorean());
    if (!font) {
        MessageBox(NULL, L"Failed to load custom font: malgun.ttf\nUsing default font.", L"Font Error", MB_OK | MB_ICONWARNING);
        io.Fonts->AddFontDefault(); // 기본 폰트 추가
    }



    // Win32와 DirectX12 백엔드 초기화
    if (!ImGui_ImplWin32_Init(hwnd) || !ImGui_ImplDX12_Init(m_Device, 3, DXGI_FORMAT_R8G8B8A8_UNORM, m_SrvDescHeap,
        m_SrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
        m_SrvDescHeap->GetGPUDescriptorHandleForHeapStart())) {
        return false;
    }

    m_Initialized = true;
    return true;
}

void UIManager::Shutdown() {
    if (m_Initialized) {
        ImGui_ImplDX12_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        m_Initialized = false;
    }
}

void UIManager::NewFrame() {
    if (!m_Initialized) return;
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}



void UIManager::Render(ID3D12GraphicsCommandList* commandList) {
    if (!m_Initialized) return;

    // [1] SRV 디스크립터 힙 바인딩
    ID3D12DescriptorHeap* descriptorHeaps[] = { m_SrvDescHeap };
    commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

    // [2] ImGui 렌더링
    ImGui::Render();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);

}



void UIManager::MainMenuUI()
{

}

void UIManager::SingleSettingUI()
{

    ImGui::SetNextWindowSize(ImVec2(FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 6 * 5));  // 원하는 고정 크기 설정
    ImGui::SetNextWindowPos(ImVec2(0, 0));  // 창 위치 설정
    ImGui::Begin(U8("플레이어 능력치 설정"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text(U8("방어 관련"));

    ImGui::PushID("hp");
    ImGui::Text(U8("체력")); ImGui::SameLine();
    ImGui::SetCursorPosX(100);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##hp", &player.hp, 0, 1000); ImGui::SameLine();
    ImGui::Text("1000");
    ImGui::PopID();

    ImGui::PushID("block");
    ImGui::Text(U8("막는 정도")); ImGui::SameLine();
    ImGui::SetCursorPosX(100);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##block", &player.block_strength, 0, 100); ImGui::SameLine();
    ImGui::Text("100");
    ImGui::PopID();

    ImGui::PushID("block_speed");
    ImGui::Text(U8("패링 속도")); ImGui::SameLine();
    ImGui::SetCursorPosX(100);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##block_speed", &player.block_speed, 0, 100); ImGui::SameLine();
    ImGui::Text("100");
    ImGui::PopID();

    ImGui::PushID("dodge_decision");
    ImGui::Text(U8("회피 판단력")); ImGui::SameLine();
    ImGui::SetCursorPosX(100);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##dodge_decision", &player.dodge_decision, 0, 100); ImGui::SameLine();
    ImGui::Text("100");
    ImGui::PopID();

    ImGui::PushID("dodge_speed");
    ImGui::Text(U8("회피 속도")); ImGui::SameLine();
    ImGui::SetCursorPosX(100);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##dodge_speed", &player.dodge_speed, 0, 100); ImGui::SameLine();
    ImGui::Text("100");
    ImGui::PopID();

    ImGui::Text(U8("점프 가능"));
    ImGui::Checkbox("##jump", &player.can_jump);

    ImGui::Text(U8("방패 종류"));
    ImGui::RadioButton(U8("없음##shield"), &player.shield_type, 0); ImGui::SameLine();
    ImGui::RadioButton(U8("중형##shield"), &player.shield_type, 1); ImGui::SameLine();
    ImGui::RadioButton(U8("대형##shield"), &player.shield_type, 2);

    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text(U8("공격 관련"));

    ImGui::PushID("damage");
    ImGui::Text(U8("공격력")); ImGui::SameLine();
    ImGui::SetCursorPosX(100);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##damage", &player.damage, 0, 100); ImGui::SameLine();
    ImGui::Text("100");
    ImGui::PopID();

    ImGui::PushID("attack_speed");
    ImGui::Text(U8("공격 속도")); ImGui::SameLine();
    ImGui::SetCursorPosX(100);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##attack_speed", &player.attack_speed, 0, 100); ImGui::SameLine();
    ImGui::Text("100");
    ImGui::PopID();

    ImGui::PushID("move_speed");
    ImGui::Text(U8("이동 속도")); ImGui::SameLine();
    ImGui::SetCursorPosX(100);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##move_speed", &player.move_speed, 0, 100); ImGui::SameLine();
    ImGui::Text("100");
    ImGui::PopID();

    ImGui::PushID("run_speed");
    ImGui::Text(U8("달리기 속도")); ImGui::SameLine();
    ImGui::SetCursorPosX(100);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##run_speed", &player.run_speed, 0, 100); ImGui::SameLine();
    ImGui::Text("100");
    ImGui::PopID();

    ImGui::Text(U8("무기 종류"));
    ImGui::RadioButton(U8("소형##weapon"), &player.weapon_type, 0); ImGui::SameLine();
    ImGui::RadioButton(U8("중형##weapon"), &player.weapon_type, 1); ImGui::SameLine();
    ImGui::RadioButton(U8("대형##weapon"), &player.weapon_type, 2);

    ImGui::Text(U8("키 병합"));
    ImGui::Checkbox("##merge", &player.key_merge);

    ImGui::End();
}

void UIManager::SingleSettingUI2()
{
    ImGui::SetNextWindowSize(ImVec2(FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 6 * 5));  // 원하는 고정 크기 설정
    ImGui::SetNextWindowPos(ImVec2(FRAME_BUFFER_WIDTH / 2, 0));  // 창 위치 설정
    ImGui::Begin(U8("보스 능력치 설정"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    static int boss_size = 0; // 0: 소형, 1: 중형, 2: 대형

    ImGui::Text(U8("보스 크기 선택"));
    ImGui::RadioButton(U8("소형"), &boss_size, 0); ImGui::SameLine();

    ImGui::BeginDisabled(true); // 비활성화 시작
    ImGui::RadioButton(U8("중형"), &boss_size, 1); ImGui::SameLine();
    ImGui::RadioButton(U8("대형"), &boss_size, 2);
    ImGui::EndDisabled();       // 비활성화 종료

    // 보스 크기에 따라 다른 UI 출력
    if (boss_size == 0) {
        ImGui::Text(U8("소형 보스 세팅"));

        ImGui::PushID("hp");
        ImGui::Text(U8("체력")); ImGui::SameLine();
        ImGui::SetCursorPosX(100);
        ImGui::Text("0"); ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::SliderInt("##hp", &monster.hp, 0, 1000); ImGui::SameLine();
        ImGui::Text("1000");
        ImGui::PopID();

        ImGui::PushID("damage");
        ImGui::Text(U8("공격력")); ImGui::SameLine();
        ImGui::SetCursorPosX(100);
        ImGui::Text("0"); ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::SliderInt("##damage", &monster.damage, 0, 100); ImGui::SameLine();
        ImGui::Text("100");
        ImGui::PopID();

        // 
        // 4. 보스 패턴 목록
        ImGui::Separator();
        ImGui::Text(U8("패턴 설정"));
        static int selected_pattern = -1;


        ImGui::Checkbox(U8("일반 베기"), &monster.use_slash);
        ImGui::Checkbox(U8("빠른 베기"), &monster.use_quick_slash);
        ImGui::Checkbox(U8("찌르기"), &monster.use_stab);
        ImGui::Checkbox(U8("강한 베기"), &monster.use_heavy_slash);
        ImGui::Checkbox(U8("돌진 베기"), &monster.use_dash_slash);
        ImGui::Checkbox(U8("확률 패링"), &monster.use_parry_chance);
        ImGui::Checkbox(U8("확률 회피"), &monster.use_dodge_chance);

    }
    else if (boss_size == 1) {
        ImGui::Text(U8("중형 보스 세팅"));
        ImGui::PushID("hp");
        ImGui::Text(U8("체력")); ImGui::SameLine();
        ImGui::SetCursorPosX(100);
        ImGui::Text("0"); ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::SliderInt("##hp", &monster.hp, 0, 1000); ImGui::SameLine();
        ImGui::Text("1000");
        ImGui::PopID();

        ImGui::PushID("damage");
        ImGui::Text(U8("공격력")); ImGui::SameLine();
        ImGui::SetCursorPosX(100);
        ImGui::Text("0"); ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::SliderInt("##damage", &monster.damage, 0, 100); ImGui::SameLine();
        ImGui::Text("100");
        ImGui::PopID();
    }
    else if (boss_size == 2) {
        ImGui::Text(U8("대형 보스 세팅"));
        ImGui::PushID("hp");
        ImGui::Text(U8("체력")); ImGui::SameLine();
        ImGui::SetCursorPosX(100);
        ImGui::Text("0"); ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::SliderInt("##hp", &monster.hp, 0, 1000); ImGui::SameLine();
        ImGui::Text("1000");
        ImGui::PopID();

        ImGui::PushID("damage");
        ImGui::Text(U8("공격력")); ImGui::SameLine();
        ImGui::SetCursorPosX(100);
        ImGui::Text("0"); ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::SliderInt("##damage", &monster.damage, 0, 100); ImGui::SameLine();
        ImGui::Text("100");
        ImGui::PopID();
        //}

        ImGui::End();
    }
}

void UIManager::SinglePlayUI()
{
    // 초기화
    if (player.hpMax == -1) player.hpMax = player.hp;
    if (monster.hpMax == -1) monster.hpMax = monster.hp;

    const float barWidth = 400.0f;
    const float barHeight = 20.0f;
    ImVec2 screenCenter = ImVec2(FRAME_BUFFER_WIDTH / 2.0f, FRAME_BUFFER_HEIGHT / 2.0f);

    float playerHealthRatio = static_cast<float>(player.hp) / static_cast<float>(player.hpMax);
    float monsterHealthRatio = static_cast<float>(monster.hp) / static_cast<float>(monster.hpMax);

    // ImGui 스타일: 빨간색 ProgressBar
    ImVec4 redColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

    // ================= 최상단 중앙 =================
    ImGui::SetNextWindowPos(ImVec2(screenCenter.x - barWidth / 2, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(barWidth, barHeight));
    ImGui::Begin("TopHealthBar", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, redColor); // 체력바 색상
    ImGui::ProgressBar(monsterHealthRatio, ImVec2(barWidth, barHeight));
    ImGui::PopStyleColor();
    ImGui::End();

    // ================= 최하단 중앙 =================
    ImGui::SetNextWindowPos(ImVec2(screenCenter.x - barWidth / 2, FRAME_BUFFER_HEIGHT - barHeight - 50), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(barWidth, barHeight));
    ImGui::Begin("PlayerHealthBar", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, redColor); // 체력바 색상
    ImGui::ProgressBar(playerHealthRatio, ImVec2(barWidth, barHeight));
    ImGui::PopStyleColor();
    ImGui::End();

    // ================= 최하단 중앙 - 스태미너 바 ================= 
    ImVec4 yellowColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // 노란색
    float playerStaminaRatio = static_cast<float>(player.stamina) / static_cast<float>(player.staminaMax);
    const float staminaBarHeight = 10.0f; // 더 얇은 스태미너 바

    ImGui::SetNextWindowPos(ImVec2(screenCenter.x - barWidth / 2, FRAME_BUFFER_HEIGHT - barHeight - 50 + barHeight), ImGuiCond_Always); // 
    ImGui::SetNextWindowSize(ImVec2(barWidth, staminaBarHeight));
    ImGui::Begin("PlayerStaminaBar", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, yellowColor); // 스태미너 색상
    ImGui::ProgressBar(playerStaminaRatio, ImVec2(barWidth-100, staminaBarHeight));
    ImGui::PopStyleColor();
    ImGui::End();

    // ================= 좌측 하단 포션 UI ================
    const float fontSize = 2.f; // 글자 크기 배율
    ImVec4 textColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // 금빛 느낌의 노란색

    // 위치 조정
    ImVec2 potionPos = ImVec2(0.0f, FRAME_BUFFER_HEIGHT-80); // 아래로 조금 내림

    ImGui::SetNextWindowPos(potionPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 80));
    ImGui::Begin("PotionUI", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 30); // 글씨를 조금 더 아래로
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);   // 글씨 색상 변경
    ImGui::SetWindowFontScale(fontSize);               // 글씨 크기 확대

    ImGui::Text("Potion : %d", player.potionCount);

    ImGui::SetWindowFontScale(1.0f); // 글씨 크기 복원
    ImGui::PopStyleColor();          // 색상 복원
    ImGui::End();

}

void UIManager::MultiSettingUI()
{
    ImGui::SetNextWindowSize(ImVec2(FRAME_BUFFER_WIDTH / 2, FRAME_BUFFER_HEIGHT / 6 * 5));  // 원하는 고정 크기 설정
    ImGui::SetNextWindowPos(ImVec2(0, 0));  // 창 위치 설정
    ImGui::Begin(U8("플레이어 능력치 설정"), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

 
    ImGui::Text(U8("방패 종류"));
    ImGui::RadioButton(U8("없음##shield"), &player.shield_type, 0); ImGui::SameLine();
    ImGui::RadioButton(U8("중형##shield"), &player.shield_type, 1); ImGui::SameLine();
    ImGui::RadioButton(U8("대형##shield"), &player.shield_type, 2);

   
    ImGui::Text(U8("무기 종류"));
    ImGui::RadioButton(U8("소형##weapon"), &player.weapon_type, 0); ImGui::SameLine();
    ImGui::RadioButton(U8("중형##weapon"), &player.weapon_type, 1); ImGui::SameLine();
    ImGui::RadioButton(U8("대형##weapon"), &player.weapon_type, 2);


    ImGui::End();

}

void UIManager::MultiPlayUI()
{
    // 초기화
    if (player.hpMax == -1) player.hpMax = player.hp;
    if (monster.hpMax == -1) monster.hpMax = monster.hp;

    const float barWidth = 400.0f;
    const float barHeight = 20.0f;
    ImVec2 screenCenter = ImVec2(FRAME_BUFFER_WIDTH / 2.0f, FRAME_BUFFER_HEIGHT / 2.0f);

    float playerHealthRatio = static_cast<float>(player.hp) / static_cast<float>(player.hpMax);

    // ImGui 스타일: 빨간색 ProgressBar
    ImVec4 redColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

    // ================= 최하단 중앙 =================
    ImGui::SetNextWindowPos(ImVec2(screenCenter.x - barWidth / 2, FRAME_BUFFER_HEIGHT - barHeight - 50), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(barWidth, barHeight));
    ImGui::Begin("PlayerHealthBar", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, redColor); // 체력바 색상
    ImGui::ProgressBar(playerHealthRatio, ImVec2(barWidth, barHeight));
    ImGui::PopStyleColor();
    ImGui::End();
    // ================= 최하단 중앙 - 스태미너 바 ================= 
    ImVec4 yellowColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // 노란색
    float playerStaminaRatio = static_cast<float>(player.stamina) / static_cast<float>(player.staminaMax);
    const float staminaBarHeight = 10.0f; // 더 얇은 스태미너 바

    ImGui::SetNextWindowPos(ImVec2(screenCenter.x - barWidth / 2, FRAME_BUFFER_HEIGHT - barHeight - 50 + barHeight), ImGuiCond_Always); // 
    ImGui::SetNextWindowSize(ImVec2(barWidth, staminaBarHeight));
    ImGui::Begin("PlayerStaminaBar", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, yellowColor); // 스태미너 색상
    ImGui::ProgressBar(playerStaminaRatio, ImVec2(barWidth - 100, staminaBarHeight), ""); // <- 퍼센트 안 보이게
    ImGui::PopStyleColor();
    ImGui::End();
}

void UIManager::DecreasePlayerHp()
{
    player.hp -= monster.damage;
    if (player.hp < 0) player.hp = 0;

}

void UIManager::DecreaseMonsterHp()
{
    monster.hp -= player.damage;
    if (monster.hp < 0) monster.hp = 0;
}



void UIManager::PrepareHpReset()
{
    player.hpMax = -1;
    monster.hpMax = -1;
}

void UIManager::UsePotion()
{
    if (player.potionCount > 0) {
        player.hp = player.hpMax;
        player.potionCount--;
    }
}

void UIManager::DecreaseStamina()
{
    player.stamina -= 10;
    if (player.stamina < 0) player.stamina = 0;
}

void UIManager::PrepareSceneData() {
    player = PlayerPreset();   // 기본 생성자로 리셋
    monster = MonsterPreset(); // 기본 생성자로 리셋
}