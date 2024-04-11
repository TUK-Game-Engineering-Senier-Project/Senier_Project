#pragma once

#include "Timer.h"
#include "Shader.h"

enum class SceneState {
	MAIN_MENU,
	SINGLE_SETTING,
	SINGLE_PLAY,
	MULTI_SETTING,
	MULTI_PLAY,
};

class CScene
{
public:
	CScene();
	~CScene();

	// ������ ���콺�� Ű���� �޽����� ó���Ѵ�.
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList);
	virtual void ReleaseObjects();

	bool ProcessInput(UCHAR* pKeysBuffer);
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	void ReleaseUploadBuffers();

	// �׷��� ��Ʈ �ñ׳��ĸ� �����Ѵ�.
	ID3D12RootSignature* CreateGraphicsRootSignature(ID3D12Device* pd3dDevice);
	ID3D12RootSignature* GetGraphicsRootSignature();

	SceneState GetSceneState() const { return m_SceneState; }

protected:
	

	//��ġ(Batch) ó���� �ϱ� ���Ͽ� ���� ���̴����� ����Ʈ�� ǥ���Ѵ�. 
	CObjectsShader *m_pShaders = NULL;
	int m_nShaders = 0;

	SceneState					m_SceneState;

	// ��Ʈ �ñ׳��ĸ� ��Ÿ���� �������̽� �������̴�.
	/*�׷��Ƚ� �����������̳� ��� ���������ο� ���� ���Ǵ� ���ҽ�
	(��: ��� ����, ���÷�, �ؽ�ó ��)�� ���� ���� ����� ����*/ 
	// ������ ���ϸ� GPU���� ���̴����� � ���ҽ��� ��� ����� ������ �˷��ִ� ���赵
	ID3D12RootSignature* m_pd3dGraphicsRootSignature = NULL;
};

//queue<CScene> g_Scene;