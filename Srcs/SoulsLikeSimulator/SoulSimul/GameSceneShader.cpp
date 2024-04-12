#include "Stdafx.h"
#include "GameSceneShader.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

CMainSceneShader::CMainSceneShader()
{

}

CMainSceneShader::~CMainSceneShader()
{
}

void CMainSceneShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CScene* pScene, void* pContext)
{
	// 배경화면 메쉬 불러오기
	CPlaneMesh* pBackGroundMesh = new CPlaneMesh(pd3dDevice, pd3dCommandList, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.f, XMFLOAT4(0.f, 0.f, 1.f, 1.f));
	// 버튼 메쉬 불러오기
	CPlaneMesh* pButtonMesh = new CPlaneMesh(pd3dDevice, pd3dCommandList, UI_MAIN_SIGLESETTING_BUTTON_WIDTH, UI_MAIN_SIGLESETTING_BUTTON_HEIGHT);

	m_nObjects = 3;
	m_ppObjects = new CGameObject * [m_nObjects];

	m_ppObjects[0] = new CGameObject();
	m_ppObjects[0]->SetMesh(0, pBackGroundMesh);
	m_ppObjects[0]->SetPosition(0, 0.f, 10.f);

	m_ppObjects[1] = new CGameObject();
	m_ppObjects[1]->SetMesh(0, pButtonMesh);
	m_ppObjects[1]->SetPosition(UI_MAIN_SIGLESETTING_BUTTON_x, UI_MAIN_SIGLESETTING_BUTTON_y, 0.f);

	m_ppObjects[2] = new CGameObject();
	m_ppObjects[2]->SetMesh(0, pButtonMesh);
	m_ppObjects[2]->SetPosition(UI_MAIN_MULTISETTING_BUTTON_x, UI_MAIN_MULTISETTING_BUTTON_y, 0.f);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

CSingleSetSceneShader::CSingleSetSceneShader()
{

}

CSingleSetSceneShader::~CSingleSetSceneShader()
{
}

void CSingleSetSceneShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CScene* pScene, void* pContext)
{
	CPlaneMesh* pBackGroundMesh = new CPlaneMesh(pd3dDevice, pd3dCommandList, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.f, XMFLOAT4(0.f, 0.f, 1.f, 1.f));
	CPlaneMesh* pButtonMesh = new CPlaneMesh(pd3dDevice, pd3dCommandList, UI_SINGLE_START_BUTTON_WIDTH, UI_SINGLE_START_BUTTON_HEIGHT);

	m_nObjects = 2;
	m_ppObjects = new CGameObject * [m_nObjects];

	m_ppObjects[0] = new CGameObject();
	m_ppObjects[0]->SetMesh(0, pBackGroundMesh);
	m_ppObjects[0]->SetPosition(0, 0.f, 10.f);

	m_ppObjects[1] = new CGameObject();
	m_ppObjects[1]->SetMesh(0, pButtonMesh);
	m_ppObjects[1]->SetPosition(UI_SINGLE_START_BUTTON_x, UI_SINGLE_START_BUTTON_y, 0.f);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

CSinglePlaySceneShader::CSinglePlaySceneShader()
{
}

CSinglePlaySceneShader::~CSinglePlaySceneShader()
{
}

void CSinglePlaySceneShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CScene* pScene, void* pContext)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

CMulitSetSceneShader::CMulitSetSceneShader()
{
}

CMulitSetSceneShader::~CMulitSetSceneShader()
{
}

void CMulitSetSceneShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
	* pd3dCommandList, CScene* pScene, void* pContext)
{
	CPlaneMesh* pBackGroundMesh = new CPlaneMesh(pd3dDevice, pd3dCommandList, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.f, XMFLOAT4(0.f, 0.f, 1.f, 1.f));
	CPlaneMesh* pButtonMesh = new CPlaneMesh(pd3dDevice, pd3dCommandList, UI_MULTI_START_BUTTON_WIDTH, UI_MULTI_START_BUTTON_HEIGHT);
	CPlaneMesh* pUIMesh = new CPlaneMesh(pd3dDevice, pd3dCommandList, UI_MULTI_MULTISETTING_INTERFACE_1_WIDTH, UI_MULTI_MULTISETTING_INTERFACE_1_HEIGHT);

	m_nObjects = 4;
	m_ppObjects = new CGameObject * [m_nObjects];

	m_ppObjects[0] = new CGameObject();
	m_ppObjects[0]->SetMesh(0, pBackGroundMesh);
	m_ppObjects[0]->SetPosition(0, 0.f, 10.f);

	m_ppObjects[1] = new CGameObject();
	m_ppObjects[1]->SetMesh(0, pButtonMesh);
	m_ppObjects[1]->SetPosition(UI_MULTI_START_BUTTON_x, UI_MULTI_START_BUTTON_y, 0.f);

	m_ppObjects[2] = new CGameObject();
	m_ppObjects[2]->SetMesh(0, pUIMesh);
	m_ppObjects[2]->SetPosition(UI_MULTI_MULTISETTING_INTERFACE_1_x, UI_MULTI_MULTISETTING_INTERFACE_1_y, 0.f);

	m_ppObjects[3] = new CGameObject();
	m_ppObjects[3]->SetMesh(0, pUIMesh);
	m_ppObjects[3]->SetPosition(UI_MULTI_MULTISETTING_INTERFACE_2_x, UI_MULTI_MULTISETTING_INTERFACE_2_y, 0.f);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

CMulitPlaySceneShader::CMulitPlaySceneShader()
{

}

CMulitPlaySceneShader::~CMulitPlaySceneShader()
{

}

void CMulitPlaySceneShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
	* pd3dCommandList, CScene* pScene, void* pContext)
{

}