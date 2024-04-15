#include "stdafx.h"
#include "GameScenes.h"
#include "GameSceneShader.h"

CMainScene::CMainScene()
{
	m_SceneState = SceneState::MAIN_MENU;
}

CMainScene::~CMainScene()
{
}

bool CMainScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CMainScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CMainScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	// 그래픽 루트 시그너쳐를 생성한다. 
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	int nObjects = 3;
	CreateCbvSrvDescriptorHeaps(pd3dDevice, nObjects + 1, UI_MAIN_TEXTURE);

	m_nShaders = 1;
	m_ppShaders = new CShader * [m_nShaders];
	CMainSceneShader* pMainSceneShader = new CMainSceneShader[m_nShaders];
	pMainSceneShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	pMainSceneShader->BuildObjects(pd3dDevice, pd3dCommandList, this, NULL);

	m_ppShaders[0] = pMainSceneShader;

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CMainScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();
	if (m_pd3dCbvSrvDescriptorHeap) m_pd3dCbvSrvDescriptorHeap->Release();

	if (m_ppShaders)
	{
		for (int i = 0; i < m_nShaders; i++)
		{
			m_ppShaders[i]->ReleaseShaderVariables();
			m_ppShaders[i]->ReleaseObjects();
			m_ppShaders[i]->Release();
		}
		delete[] m_ppShaders;
	}

	ReleaseShaderVariables();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

CSingleSettingScene::CSingleSettingScene()
{
	m_SceneState = SceneState::SINGLE_SETTING;
}

CSingleSettingScene::~CSingleSettingScene()
{
}

bool CSingleSettingScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CSingleSettingScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CSingleSettingScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	// 그래픽 루트 시그너쳐를 생성한다. 
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	int nObjects = 2;
	CreateCbvSrvDescriptorHeaps(pd3dDevice, nObjects + 1, UI_SINGLE_TEXTURE);

	m_nShaders = 1;
	m_ppShaders = new CShader * [m_nShaders];
	CSingleSetSceneShader* pSingleSetSceneShader = new CSingleSetSceneShader[m_nShaders];
	pSingleSetSceneShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	pSingleSetSceneShader->BuildObjects(pd3dDevice, pd3dCommandList, this, NULL);

	m_ppShaders[0] = pSingleSetSceneShader;

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CSingleSettingScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();
	if (m_pd3dCbvSrvDescriptorHeap) m_pd3dCbvSrvDescriptorHeap->Release();

	if (m_ppShaders)
	{
		for (int i = 0; i < m_nShaders; i++)
		{
			m_ppShaders[i]->ReleaseShaderVariables();
			m_ppShaders[i]->ReleaseObjects();
			m_ppShaders[i]->Release();
		}
		delete[] m_ppShaders;
	}

	ReleaseShaderVariables();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

CSinglePlayScene::CSinglePlayScene()
{
	m_SceneState = SceneState::SINGLE_PLAY;
}

CSinglePlayScene::~CSinglePlayScene()
{
}

bool CSinglePlayScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CSinglePlayScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CSinglePlayScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	// 그래픽 루트 시그너쳐를 생성한다. 
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
}

void CSinglePlayScene::ReleaseObjects()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

CMultiSettingScene::CMultiSettingScene()
{
	m_SceneState = SceneState::MULTI_SETTING;
}

CMultiSettingScene::~CMultiSettingScene()
{
}

bool CMultiSettingScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CMultiSettingScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CMultiSettingScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	// 그래픽 루트 시그너쳐를 생성한다. 
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	int nObjects = 4;
	CreateCbvSrvDescriptorHeaps(pd3dDevice, nObjects + 1, UI_MULTI_TEXTURE);

	m_nShaders = 1;
	m_ppShaders = new CShader * [m_nShaders];
	CMulitSetSceneShader* pMulitSetSceneShader = new CMulitSetSceneShader[m_nShaders];
	pMulitSetSceneShader->CreateShader(pd3dDevice, m_pd3dGraphicsRootSignature);
	pMulitSetSceneShader->BuildObjects(pd3dDevice, pd3dCommandList, this, NULL);

	m_ppShaders[0] = pMulitSetSceneShader;

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CMultiSettingScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();
	if (m_pd3dCbvSrvDescriptorHeap) m_pd3dCbvSrvDescriptorHeap->Release();

	if (m_ppShaders)
	{
		for (int i = 0; i < m_nShaders; i++)
		{
			m_ppShaders[i]->ReleaseShaderVariables();
			m_ppShaders[i]->ReleaseObjects();
			m_ppShaders[i]->Release();
		}
		delete[] m_ppShaders;
	}

	ReleaseShaderVariables();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

CMultiPlayScene::CMultiPlayScene()
{
	m_SceneState = SceneState::MULTI_PLAY;
}

CMultiPlayScene::~CMultiPlayScene()
{
}

bool CMultiPlayScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CMultiPlayScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CMultiPlayScene::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList)
{
	// 그래픽 루트 시그너쳐를 생성한다. 
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
}

void CMultiPlayScene::ReleaseObjects()
{
}
