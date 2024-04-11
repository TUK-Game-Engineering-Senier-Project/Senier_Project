#pragma once
#include "GameScenes.h"


class CSceneManager
{
private:
	stack<shared_ptr<CScene>>	m_qScenes;
	SceneState					m_CurrentState;

public:
	CSceneManager();
	virtual ~CSceneManager();

	void StartScene(const SceneState fSceneState);
	void EndCurrentScene();

	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);
	virtual bool OnProcessingKeyboardMessage(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, 
		HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList);
	virtual void ReleaseObjects();

	bool ProcessInput(UCHAR* pKeysBuffer);
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	void ReleaseUploadBuffers();

	ID3D12RootSignature* GetGraphicsRootSignature();

	SceneState GetCurrentSceneState() const { return m_CurrentState; }
};

