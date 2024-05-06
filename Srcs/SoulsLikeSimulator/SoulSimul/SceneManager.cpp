#include "Stdafx.h"
#include "SceneManager.h"

CSceneManager::CSceneManager()
{
	shared_ptr<CScene> pScene = make_shared<CMainScene>();
	//shared_ptr<CScene> pScene = make_shared<CMultiSettingScene>();
	m_CurrentState = pScene->GetSceneState();

	m_qScenes.push(pScene);
}

CSceneManager::~CSceneManager()
{
}

void CSceneManager::StartScene(const SceneState fSceneState)
{
	shared_ptr<CScene> pScene;

	switch (fSceneState)
	{
	case SceneState::MAIN_MENU:
		pScene = make_shared<CMainScene>();
		m_CurrentState = pScene->GetSceneState();
		m_qScenes.push(pScene);
		break;

	case SceneState::SINGLE_SETTING:
		pScene = make_shared<CSingleSettingScene>();
		m_CurrentState = pScene->GetSceneState();
		m_qScenes.push(pScene);
		break;

	case SceneState::SINGLE_PLAY:
		pScene = make_shared<CSinglePlayScene>();
		m_CurrentState = pScene->GetSceneState();
		m_qScenes.push(pScene);
		break;

	case SceneState::MULTI_SETTING:
		pScene = make_shared<CMultiSettingScene>();
		m_CurrentState = pScene->GetSceneState();
		m_qScenes.push(pScene);
		break;

	case SceneState::MULTI_PLAY:
		pScene = make_shared<CMultiPlayScene>();
		m_CurrentState = pScene->GetSceneState();
		m_qScenes.push(pScene);
		break;
	default:
		break;
	}
}

void CSceneManager::EndCurrentScene()
{
	if (m_qScenes.size() > 1)
	{
		m_qScenes.pop();
		m_CurrentState = m_qScenes.top()->GetSceneState();
	}
}

bool CSceneManager::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	bool b_change = false;
	shared_ptr<CScene> pScene;

	// 클릭한 좌표
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		::SetCapture(hWnd);
		if (m_CurrentState == SceneState::MAIN_MENU) {

			// 클릭한 좌표가 싱글 플레이 버튼의 영역에 속하는지 확인
			if (x >= UI_MAIN_SIGLESETTING_BUTTON_x + 250 &&
				x <= UI_MAIN_SIGLESETTING_BUTTON_x + UI_MAIN_SIGLESETTING_BUTTON_WIDTH  +250 &&
				y >= UI_MAIN_SIGLESETTING_BUTTON_y + 250 &&
				y <= UI_MAIN_SIGLESETTING_BUTTON_y + UI_MAIN_SIGLESETTING_BUTTON_HEIGHT + 250) {
				StartScene(SceneState::SINGLE_SETTING);
				b_change = true;
			}
			else if (x >= UI_MAIN_MULTISETTING_BUTTON_x + 250 &&
				x <= UI_MAIN_MULTISETTING_BUTTON_x + UI_MAIN_MULTISETTING_BUTTON_WIDTH + 250 &&
				y >= UI_MAIN_MULTISETTING_BUTTON_y + 250 +(2*150) &&
				y <= UI_MAIN_MULTISETTING_BUTTON_y + UI_MAIN_MULTISETTING_BUTTON_HEIGHT + 250 + (2 * 150)) {
				StartScene(SceneState::MULTI_SETTING);
				b_change = true;
			}
		}
		
		break;
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
		::ReleaseCapture();
		break;
	case WM_RBUTTONUP:
		
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}

	if (m_qScenes.top()->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam))
		b_change = true;
	return b_change;
}


bool CSceneManager::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	bool b_change = false;
	shared_ptr<CScene> pScene;

	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case '2':
			if (m_CurrentState == SceneState::MAIN_MENU) {
				StartScene(SceneState::SINGLE_SETTING);
				b_change = true;
			}
			break;

		case '3':
			if (m_CurrentState == SceneState::MAIN_MENU) {
				StartScene(SceneState::MULTI_SETTING);
				b_change = true;
			}
			break;

		case VK_ESCAPE:
			if (m_CurrentState != SceneState::MAIN_MENU) {
				EndCurrentScene();
			}
			break;

		default:
			break;
		}
		break;
	default:
		break;
	}
	if (m_qScenes.top()->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam))
		b_change = true;
	return b_change;
}

void CSceneManager::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	m_qScenes.top()->BuildObjects(pd3dDevice, pd3dCommandList);
}

void CSceneManager::ReleaseObjects()
{
	m_qScenes.top()->ReleaseObjects();
}

D3D12_GPU_DESCRIPTOR_HANDLE CSceneManager::CreateConstantBufferView(ID3D12Device* pd3dDevice, ID3D12Resource* pd3dConstantBuffer, UINT nStride)
{
	return m_qScenes.top()->CreateConstantBufferView(pd3dDevice, pd3dConstantBuffer, nStride);
}

D3D12_GPU_DESCRIPTOR_HANDLE CSceneManager::CreateConstantBufferView(ID3D12Device* pd3dDevice, D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress, UINT nStride)
{
	return m_qScenes.top()->CreateConstantBufferView(pd3dDevice, d3dGpuVirtualAddress, nStride);
}

bool CSceneManager::ProcessInput(UCHAR* pKeysBuffer)
{
	//if (::GetKeyboardState(pKeysBuffer))
	//{
	//	if (pKeysBuffer['2'] & 0xF0 && m_CurrentState == SceneState::MAIN_MENU)
	//		StartScene(SceneState::SINGLE_SETTING);
	//	if (pKeysBuffer['3'] & 0xF0 && m_CurrentState == SceneState::MAIN_MENU)
	//		StartScene(SceneState::MULTI_SETTING);
	//	if (pKeysBuffer[VK_ESCAPE] & 0xF0 && m_CurrentState != SceneState::MAIN_MENU)
	//		EndCurrentScene();
	//}
	return m_qScenes.top()->ProcessInput(pKeysBuffer);
}

void CSceneManager::AnimateObjects(float fTimeElapsed)
{
	m_qScenes.top()->AnimateObjects(fTimeElapsed);
}

void CSceneManager::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
	m_qScenes.top()->Render(pd3dCommandList, pCamera);
}

void CSceneManager::ReleaseUploadBuffers()
{
	m_qScenes.top()->ReleaseUploadBuffers();
}

ID3D12RootSignature* CSceneManager::GetGraphicsRootSignature()
{
	return m_qScenes.top()->GetGraphicsRootSignature();
}

