#include "UIManager.h"
#include "define.h"
#include <DirectXMath.h>  // 추가
using namespace DirectX;   // 추가


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
#define U8(text) reinterpret_cast<const char*>(u8##text)
void UIManager::SingleSettingUI()
{
    // UI 디버그 창
    ImGui::SetNextWindowSize(ImVec2(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT / 5 * 4));  // 원하는 고정 크기 설정
    ImGui::SetNextWindowPos(ImVec2(0, 0));  // 창 위치 설정
    ImGui::Begin(U8("플레이어 능력치 설정"), nullptr, ImGuiWindowFlags_NoCollapse);

    ImGui::Text(U8("방어 관련"));

    ImGui::PushID("hp");
    ImGui::Text(U8("체력")); ImGui::SameLine();
    ImGui::SetCursorPosX(150);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##hp", &player.hp, 0, 1000); ImGui::SameLine();
    ImGui::Text("1000");
    ImGui::PopID();

    ImGui::PushID("block");
    ImGui::Text(U8("막는 정도")); ImGui::SameLine();
    ImGui::SetCursorPosX(150);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##block", &player.block_strength, 0, 100); ImGui::SameLine();
    ImGui::Text("100");
    ImGui::PopID();

    ImGui::PushID("block_speed");
    ImGui::Text(U8("패링 속도")); ImGui::SameLine();
    ImGui::SetCursorPosX(150);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##block_speed", &player.block_speed, 0, 100); ImGui::SameLine();
    ImGui::Text("100");
    ImGui::PopID();

    ImGui::PushID("dodge_decision");
    ImGui::Text(U8("회피 판단력")); ImGui::SameLine();
    ImGui::SetCursorPosX(150);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##dodge_decision", &player.dodge_decision, 0, 100); ImGui::SameLine();
    ImGui::Text("100");
    ImGui::PopID();

    ImGui::PushID("dodge_speed");
    ImGui::Text(U8("회피 속도")); ImGui::SameLine();
    ImGui::SetCursorPosX(150);
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
    ImGui::SetCursorPosX(150);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##damage", &player.damage, 0, 100); ImGui::SameLine();
    ImGui::Text("100");
    ImGui::PopID();

    ImGui::PushID("attack_speed");
    ImGui::Text(U8("공격 속도")); ImGui::SameLine();
    ImGui::SetCursorPosX(150);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##attack_speed", &player.attack_speed, 0, 100); ImGui::SameLine();
    ImGui::Text("100");
    ImGui::PopID();

    ImGui::PushID("move_speed");
    ImGui::Text(U8("이동 속도")); ImGui::SameLine();
    ImGui::SetCursorPosX(150);
    ImGui::Text("0"); ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::SliderInt("##move_speed", &player.move_speed, 0, 100); ImGui::SameLine();
    ImGui::Text("100");
    ImGui::PopID();

    ImGui::PushID("run_speed");
    ImGui::Text(U8("달리기 속도")); ImGui::SameLine();
    ImGui::SetCursorPosX(150);
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

void UIManager::SinglePlayUI()
{
    if (player.hpMax == -1) player.hpMax = player.hp;  // 최초 한 번만 초기화

    const float barWidth = 400.0f;
    const float barHeight = 20.0f;

    ImVec2 screenCenter = ImVec2(FRAME_BUFFER_WIDTH / 2.0f, FRAME_BUFFER_HEIGHT / 2.0f);
    float healthRatio = static_cast<float>(player.hp) / static_cast<float>(player.hpMax);

    // ImGui 스타일: 빨간색 ProgressBar
    ImVec4 redColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

    // ================= 최상단 중앙 =================
    ImGui::SetNextWindowPos(ImVec2(screenCenter.x - barWidth / 2, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(barWidth, barHeight));
    ImGui::Begin("TopHealthBar", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, redColor); // 체력바 색상
    ImGui::ProgressBar(healthRatio, ImVec2(barWidth, barHeight));
    ImGui::PopStyleColor();
    ImGui::End();

    // ================= 최하단 중앙 =================
    ImGui::SetNextWindowPos(ImVec2(screenCenter.x - barWidth / 2, FRAME_BUFFER_HEIGHT - barHeight - 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(barWidth, barHeight));
    ImGui::Begin("PlayerHealthBar", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, redColor); // 체력바 색상
    ImGui::ProgressBar(healthRatio, ImVec2(barWidth, barHeight));
    ImGui::PopStyleColor();
    ImGui::End();
}

void UIManager::MultiSettingUI()
{
}

void UIManager::MultiPlayUI()
{
}

void UIManager::DecreasePlayerHp()
{
    player.hp -= player.damage;
    if (player.hp < 0) player.hp = 0;
}

void UIManager::PrepareHpReset()
{
    player.hpMax = -1;
}

