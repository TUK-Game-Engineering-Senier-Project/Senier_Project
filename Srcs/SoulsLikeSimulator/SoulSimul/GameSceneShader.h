#pragma once
#include "Shader.h"

// 여기 부터는 각 씬들에 맞게 게임 객체들(UI)들을 포함하는 객체이다.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

class CMainSceneShader : public CObjectsShader
{
public:
	CMainSceneShader();
	virtual ~CMainSceneShader();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList, CScene* pScene, void* pContext = NULL);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

class CSingleSetSceneShader : public CObjectsShader
{
public:
	CSingleSetSceneShader();
	virtual ~CSingleSetSceneShader();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList, CScene* pScene, void* pContext = NULL);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

class CSinglePlaySceneShader : public CObjectsShader
{
public:
	CSinglePlaySceneShader();
	virtual ~CSinglePlaySceneShader();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList, CScene* pScene, void* pContext = NULL);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

class CMulitSetSceneShader : public CObjectsShader
{
public:
	CMulitSetSceneShader();
	virtual ~CMulitSetSceneShader();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList, CScene* pScene, void* pContext = NULL);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

class CMulitPlaySceneShader : public CObjectsShader
{
public:
	CMulitPlaySceneShader();
	virtual ~CMulitPlaySceneShader();

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList, CScene* pScene, void* pContext = NULL);
};