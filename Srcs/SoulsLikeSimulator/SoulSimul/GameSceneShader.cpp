#include "Stdafx.h"
#include "GameSceneShader.h"
#include "Scene.h"
#include "WICTextureLoader12.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

CMainSceneShader::CMainSceneShader()
{

}

CMainSceneShader::~CMainSceneShader()
{
}

// ui 이미지
void CMainSceneShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, CScene* pScene, void* pContext)
{
	CTexture* ppTextures[UI_MAIN_TEXTURE];
	// 배경화면
	ppTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	ppTextures[0]->LoadTextureFromWICFile(pd3dDevice, pd3dCommandList, L"Resource/UI/MainScreen.png", RESOURCE_TEXTURE2D, 0);
	// 버튼
	ppTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	ppTextures[1]->LoadTextureFromWICFile(pd3dDevice, pd3dCommandList, L"Resource/UI/SinglePlay.png", RESOURCE_TEXTURE2D, 0);
	// 버튼
	ppTextures[2] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	ppTextures[2]->LoadTextureFromWICFile(pd3dDevice, pd3dCommandList, L"Resource/UI/MultiPlay.png", RESOURCE_TEXTURE2D, 0);


	m_nObjects = 3;
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	for (int i = 0; i < UI_MAIN_TEXTURE; i++) pScene->CreateShaderResourceViews(pd3dDevice, ppTextures[i], 0, 3);

	CMaterial* ppMaterials[UI_MAIN_TEXTURE];
	for (int i = 0; i < UI_MAIN_TEXTURE; i++)
	{
		ppMaterials[i] = new CMaterial();
		ppMaterials[i]->SetTexture(ppTextures[i]);
	}

	CPlaneTextureMesh* pBackGroundMesh = new CPlaneTextureMesh(pd3dDevice, pd3dCommandList, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.f);
	CPlaneTextureMesh* pButton1Mesh = new CPlaneTextureMesh(pd3dDevice, pd3dCommandList, UI_MAIN_SIGLESETTING_BUTTON_WIDTH, UI_MAIN_SIGLESETTING_BUTTON_HEIGHT, 0.f);
	CPlaneTextureMesh* pButton2Mesh = new CPlaneTextureMesh(pd3dDevice, pd3dCommandList, UI_MAIN_MULTISETTING_BUTTON_WIDTH, UI_MAIN_MULTISETTING_BUTTON_HEIGHT, 0.f);

	m_ppObjects = new CGameObject * [m_nObjects];

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbGameObjects->GetGPUVirtualAddress();

	m_ppObjects[0] = new CGameObject();
	m_ppObjects[0]->SetMesh(0, pBackGroundMesh);
	m_ppObjects[0]->SetMaterial(ppMaterials[0]);
	m_ppObjects[0]->SetPosition(0, 0.f, 10.f);
	D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle = pScene->CreateConstantBufferView(pd3dDevice, d3dGpuVirtualAddress + (((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255) * 0), ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255));
	m_ppObjects[0]->SetCbvGPUDescriptorHandle(d3dCbvGPUDescriptorHandle);

	m_ppObjects[1] = new CGameObject();
	m_ppObjects[1]->SetMesh(0, pButton1Mesh);
	m_ppObjects[1]->SetMaterial(ppMaterials[1]);
	m_ppObjects[1]->SetPosition(UI_MAIN_SIGLESETTING_BUTTON_x, UI_MAIN_SIGLESETTING_BUTTON_y, 0.f);
	d3dCbvGPUDescriptorHandle = pScene->CreateConstantBufferView(pd3dDevice, d3dGpuVirtualAddress + (((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255) * 1), ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255));
	m_ppObjects[1]->SetCbvGPUDescriptorHandle(d3dCbvGPUDescriptorHandle);

	m_ppObjects[2] = new CGameObject();
	m_ppObjects[2]->SetMesh(0, pButton2Mesh);
	m_ppObjects[2]->SetMaterial(ppMaterials[2]);
	m_ppObjects[2]->SetPosition(UI_MAIN_MULTISETTING_BUTTON_x, UI_MAIN_MULTISETTING_BUTTON_y, 0.f);
	d3dCbvGPUDescriptorHandle = pScene->CreateConstantBufferView(pd3dDevice, d3dGpuVirtualAddress + (((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255) * 2), ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255));
	m_ppObjects[2]->SetCbvGPUDescriptorHandle(d3dCbvGPUDescriptorHandle);

	
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
	CTexture* ppTextures[UI_SINGLE_TEXTURE];

	// 버튼
	ppTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	ppTextures[0]->LoadTextureFromWICFile(pd3dDevice, pd3dCommandList, L"Resource/UI/GamePlay.png", RESOURCE_TEXTURE2D, 0);

	// 배경화면 - 비활성
	ppTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	ppTextures[1]->LoadTextureFromWICFile(pd3dDevice, pd3dCommandList, L"Resource/UI/sample-background.jpg", RESOURCE_TEXTURE2D, 0);



	m_nObjects = 1; // 
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	for (int i = 0; i < UI_SINGLE_TEXTURE; i++) pScene->CreateShaderResourceViews(pd3dDevice, ppTextures[i], 0, 3);

	CMaterial* ppMaterials[UI_SINGLE_TEXTURE];
	for (int i = 0; i < UI_SINGLE_TEXTURE; i++)
	{
		ppMaterials[i] = new CMaterial();
		ppMaterials[i]->SetTexture(ppTextures[i]);
	}

	// 배경 비활성
	//CPlaneTextureMesh* pBackGroundMesh = new CPlaneTextureMesh(pd3dDevice, pd3dCommandList, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.f);
	CPlaneTextureMesh* pButtonMesh = new CPlaneTextureMesh(pd3dDevice, pd3dCommandList, UI_PLAY_START_BUTTON_WIDTH, UI_PLAY_START_BUTTON_HEIGHT, 0.f);

	m_ppObjects = new CGameObject * [m_nObjects];

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbGameObjects->GetGPUVirtualAddress();

	D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle;

	m_ppObjects[0] = new CGameObject();
	m_ppObjects[0]->SetMesh(0, pButtonMesh);
	m_ppObjects[0]->SetMaterial(ppMaterials[0]);
	m_ppObjects[0]->SetPosition(UI_PLAY_START_BUTTON_x, UI_PLAY_START_BUTTON_y, 0.f);
	d3dCbvGPUDescriptorHandle = pScene->CreateConstantBufferView(pd3dDevice, d3dGpuVirtualAddress + (((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255) * 0), ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255));
	m_ppObjects[0]->SetCbvGPUDescriptorHandle(d3dCbvGPUDescriptorHandle);

	//m_ppObjects[1] = new CGameObject();
	//m_ppObjects[1]->SetMesh(0, pBackGroundMesh);
	//m_ppObjects[1]->SetMaterial(ppMaterials[1]);
	//m_ppObjects[1]->SetPosition(0, 0.f, 10.f);
	//d3dCbvGPUDescriptorHandle = pScene->CreateConstantBufferView(pd3dDevice, d3dGpuVirtualAddress + (((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255) * 1), ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255));
	//m_ppObjects[1]->SetCbvGPUDescriptorHandle(d3dCbvGPUDescriptorHandle);
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
	CTexture* ppTextures[UI_SINGLE_PLAY_TEXTURE];


	ppTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	ppTextures[0]->LoadTextureFromWICFile(pd3dDevice, pd3dCommandList, L"Resource/UI/potion.png", RESOURCE_TEXTURE2D, 0);

	
	m_nObjects = 1; // 
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	for (int i = 0; i < UI_SINGLE_PLAY_TEXTURE; i++) pScene->CreateShaderResourceViews(pd3dDevice, ppTextures[i], 0, 3);

	CMaterial* ppMaterials[UI_SINGLE_PLAY_TEXTURE];
	for (int i = 0; i < UI_SINGLE_PLAY_TEXTURE; i++)
	{
		ppMaterials[i] = new CMaterial();
		ppMaterials[i]->SetTexture(ppTextures[i]);
	}

	CPlaneTextureMesh* pItemMesh = new CPlaneTextureMesh(pd3dDevice, pd3dCommandList, UI_POTION_WIDTH, UI_POTION_HEIGHT, 0.f);

	m_ppObjects = new CGameObject * [m_nObjects];

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbGameObjects->GetGPUVirtualAddress();

	D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle;

	m_ppObjects[0] = new CGameObject();
	m_ppObjects[0]->SetMesh(0, pItemMesh);
	m_ppObjects[0]->SetMaterial(ppMaterials[0]);
	m_ppObjects[0]->SetPosition(UI_POTION_x, UI_POTION_y, 10.f);
	d3dCbvGPUDescriptorHandle = pScene->CreateConstantBufferView(pd3dDevice, d3dGpuVirtualAddress + (((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255) * 0), ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255));
	m_ppObjects[0]->SetCbvGPUDescriptorHandle(d3dCbvGPUDescriptorHandle);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

CMultiSetSceneShader::CMultiSetSceneShader()
{
}

CMultiSetSceneShader::~CMultiSetSceneShader()
{
}

void CMultiSetSceneShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
	* pd3dCommandList, CScene* pScene, void* pContext)
{
	CTexture* ppTextures[UI_MULTI_TEXTURE];

	// 버튼
	ppTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	ppTextures[0]->LoadTextureFromWICFile(pd3dDevice, pd3dCommandList, L"Resource/UI/GamePlay.png", RESOURCE_TEXTURE2D, 0);

	// 배경화면 - 비활성
	ppTextures[1] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	ppTextures[1]->LoadTextureFromWICFile(pd3dDevice, pd3dCommandList, L"Resource/UI/sample-background.jpg", RESOURCE_TEXTURE2D, 0);



	m_nObjects = 1; // 
	CreateShaderVariables(pd3dDevice, pd3dCommandList);
	for (int i = 0; i < UI_MULTI_TEXTURE; i++) pScene->CreateShaderResourceViews(pd3dDevice, ppTextures[i], 0, 3);

	CMaterial* ppMaterials[UI_MULTI_TEXTURE];
	for (int i = 0; i < UI_MULTI_TEXTURE; i++)
	{
		ppMaterials[i] = new CMaterial();
		ppMaterials[i]->SetTexture(ppTextures[i]);
	}

	// 배경 비활성
	//CPlaneTextureMesh* pBackGroundMesh = new CPlaneTextureMesh(pd3dDevice, pd3dCommandList, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.f);
	CPlaneTextureMesh* pButtonMesh = new CPlaneTextureMesh(pd3dDevice, pd3dCommandList, UI_PLAY_START_BUTTON_WIDTH, UI_PLAY_START_BUTTON_HEIGHT, 0.f);

	m_ppObjects = new CGameObject * [m_nObjects];

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbGameObjects->GetGPUVirtualAddress();

	D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle;

	m_ppObjects[0] = new CGameObject();
	m_ppObjects[0]->SetMesh(0, pButtonMesh);
	m_ppObjects[0]->SetMaterial(ppMaterials[0]);
	m_ppObjects[0]->SetPosition(UI_PLAY_START_BUTTON_x, UI_PLAY_START_BUTTON_y, 0.f);
	d3dCbvGPUDescriptorHandle = pScene->CreateConstantBufferView(pd3dDevice, d3dGpuVirtualAddress + (((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255) * 0), ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255));
	m_ppObjects[0]->SetCbvGPUDescriptorHandle(d3dCbvGPUDescriptorHandle);

	//m_ppObjects[1] = new CGameObject();
	//m_ppObjects[1]->SetMesh(0, pBackGroundMesh);
	//m_ppObjects[1]->SetMaterial(ppMaterials[1]);
	//m_ppObjects[1]->SetPosition(0, 0.f, 10.f);
	//d3dCbvGPUDescriptorHandle = pScene->CreateConstantBufferView(pd3dDevice, d3dGpuVirtualAddress + (((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255) * 1), ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255));
	//m_ppObjects[1]->SetCbvGPUDescriptorHandle(d3dCbvGPUDescriptorHandle); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

CMultiPlaySceneShader::CMultiPlaySceneShader()
{
	
}

CMultiPlaySceneShader::~CMultiPlaySceneShader()
{

}

void CMultiPlaySceneShader::BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
	* pd3dCommandList, CScene* pScene, void* pContext)
{
	//CTexture* ppTextures[UI_MULTI_PLAY_TEXTURE];


	//ppTextures[0] = new CTexture(1, RESOURCE_TEXTURE2D, 0, 1);
	//ppTextures[0]->LoadTextureFromWICFile(pd3dDevice, pd3dCommandList, L"Resource/UI/potion.png", RESOURCE_TEXTURE2D, 0);


	//m_nObjects = 1; // 
	//CreateShaderVariables(pd3dDevice, pd3dCommandList);
	//for (int i = 0; i < UI_MULTI_PLAY_TEXTURE; i++) pScene->CreateShaderResourceViews(pd3dDevice, ppTextures[i], 0, 3);

	//CMaterial* ppMaterials[UI_MULTI_PLAY_TEXTURE];
	//for (int i = 0; i < UI_MULTI_PLAY_TEXTURE; i++)
	//{
	//	ppMaterials[i] = new CMaterial();
	//	ppMaterials[i]->SetTexture(ppTextures[i]);
	//}

	//CPlaneTextureMesh* pItemMesh = new CPlaneTextureMesh(pd3dDevice, pd3dCommandList, UI_POTION_WIDTH, UI_POTION_HEIGHT, 0.f);

	//m_ppObjects = new CGameObject * [m_nObjects];

	//D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbGameObjects->GetGPUVirtualAddress();

	//D3D12_GPU_DESCRIPTOR_HANDLE d3dCbvGPUDescriptorHandle;

	//m_ppObjects[0] = new CGameObject();
	//m_ppObjects[0]->SetMesh(0, pItemMesh);
	//m_ppObjects[0]->SetMaterial(ppMaterials[0]);
	//m_ppObjects[0]->SetPosition(UI_POTION_x, UI_POTION_y, 10.f);
	//d3dCbvGPUDescriptorHandle = pScene->CreateConstantBufferView(pd3dDevice, d3dGpuVirtualAddress + (((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255) * 0), ((sizeof(CB_GAMEOBJECT_INFO) + 255) & ~255));
	//m_ppObjects[0]->SetCbvGPUDescriptorHandle(d3dCbvGPUDescriptorHandle);
}