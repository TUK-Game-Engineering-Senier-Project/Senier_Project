#pragma once

#include "Camera.h"
#include "SceneManager.h"
#include "Timer.h"
#include "Player.h"

class CGameFramework
{
public:
	CGameFramework();
	~CGameFramework();

	//�����ӿ�ũ�� �ʱ�ȭ�ϴ� �Լ��̴�(�� �����찡 �����Ǹ� ȣ��ȴ�). 
	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	void OnDestroy();

	//���� ü��, ����̽�, ������ ��, ��� ť/�Ҵ���/����Ʈ�� �����ϴ� �Լ��̴�. 
	//���� Ÿ�� ��� ����-���ٽ� �並 �����ϴ� �Լ��̴�. 
	void CreateSwapChain();
	void CreateDirect3DDevice();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateRenderTargetViews();
	void CreateDepthStencilView();
	void CreateCommandQueueAndList();

	void OnResizeBackBuffers();

	//�������� �޽��� ���� ��ü�� �����ϰ� �Ҹ��ϴ� �Լ��̴�. 
	void BuildObjects();
	void ReleaseObjects();

	//�����ӿ�ũ�� �ٽ�(����� �Է�, �ִϸ��̼�, ������)�� �����ϴ� �Լ��̴�. 
	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();

	// ��ü ȭ��
	//void ChangeSwapChainState();

	//CPU�� GPU�� ����ȭ�ϴ� �Լ��̴�. 
	void WaitForGpuComplete();

	// ���� ���� �Լ�
	void MoveToNextFrame();

	//�������� �޽���(Ű����, ���콺 �Է�)�� ó���ϴ� �Լ��̴�.
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE					m_hInstance;
	HWND						m_hWnd;

	int							m_nWndClientWidth;
	int							m_nWndClientHeight;

	// DXGI ���丮 �������̽��� ���� �������̴�.
	IDXGIFactory4*				m_pdxgiFactory = NULL;
	// ���� ü�� �������̽��� ���� �������̴�. �ַ� ���÷��̸� �����ϱ� �ʿ��ϴ�.
	IDXGISwapChain3*			m_pdxgiSwapChain = NULL;
	// Direct3D ����̽� �������̽��� ���� �������̴�. �ַ� ���ҽ��� �����ϱ� ���Ͽ� �ʿ��ϴ�.
	ID3D12Device*				m_pd3dDevice = NULL;

	// MSAA ���� ���ø��� Ȱ��ȭ�ϰ� ���� ���ø� ������ �����Ѵ�.
	bool						m_bMsaa4xEnable = false;
	UINT						m_nMsaa4xQualityLevels = 0;

	// ���� ü���� �ĸ� ������ �����̴�.
	static const UINT			m_nSwapChainBuffers = 2;
	// ���� ����ü���� �ĸ� ������ �ε����̴�.
	UINT						m_nSwapChainBufferIndex;

	// ���� Ÿ�� ����, ������ �� �������̽� ������, ���� Ÿ�� ������ ������ ũ���̴�.
	ID3D12Resource*				m_ppd3dSwapChainBackBuffers[m_nSwapChainBuffers];
	ID3D12DescriptorHeap*		m_pd3dRtvDescriptorHeap = NULL;
	UINT						m_nRtvDescriptorIncrementSize;

	// ����-���ٽ� ����, ������ �� �������̽� ������, ����-���ٽ� ������ ������ ũ���̴�.
	ID3D12Resource*				m_pd3dDepthStencilBuffer = NULL;
	ID3D12DescriptorHeap*		m_pd3dDsvDescriptorHeap = NULL;
	UINT						m_nDsvDescriptorIncrementSize;

	// ��� ť, ��� �Ҵ���, ��� ����Ʈ �������̽� �������̴�.
	ID3D12CommandAllocator*		m_pd3dCommandAllocator = NULL;
	ID3D12CommandQueue*			m_pd3dCommandQueue = NULL;
	ID3D12GraphicsCommandList*	m_pd3dCommandList = NULL;

	// �潺 �������̽� ������, �潺�� ��, �̺�Ʈ �ڵ��̴�. 
	// �潺�� GPU�� CPU�� ����ȭ �����̴�. 
	// GPU�� ��� ��⿭�� ��ɵ� �� Ư�� ���������� ��� ����� �� ó���� ������ CPU�� ��ٸ��� �ϴ� ��
	ID3D12Fence*				m_pd3dFence = NULL;
	UINT64						m_nFenceValues[m_nSwapChainBuffers];
	HANDLE						m_hFenceEvent;

#if defined(_DEBUG)
	ID3D12Debug* m_pd3dDebugController;
#endif

	//������ ���� �����ӿ�ũ���� ����� Ÿ�̸��̴�. 
	CGameTimer					m_GameTimer;
	// ���� ���� ����
	shared_ptr<CSceneManager>	m_pScene = NULL;
	//CScene*						m_pScene = NULL;
	CCamera*					m_pCamera = NULL;
	//�÷��̾� ��ü�� ���� �������̴�.
	CPlayer*					m_pPlayer = NULL;

	//���������� ���콺 ��ư�� Ŭ���� ���� ���콺 Ŀ���� ��ġ�̴�. 
	POINT						m_ptOldCursorPos;

	//������ ������ ����Ʈ�� �� �������� ĸ�ǿ� ����ϱ� ���� ���ڿ��̴�. 
	_TCHAR						m_pszFrameRate[50];
};

