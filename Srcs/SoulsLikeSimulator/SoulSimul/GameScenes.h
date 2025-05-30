#pragma once
#include "Scene.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

class CMainScene : public CScene
{
public:
	CMainScene();
	~CMainScene();

	// 씬에서 마우스와 키보드 메시지를 처리한다.
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList);
	virtual void ReleaseObjects();

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

class CSingleSettingScene : public CScene
{
public:
	CSingleSettingScene();
	~CSingleSettingScene();

	// 씬에서 마우스와 키보드 메시지를 처리한다.
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList);
	virtual void ReleaseObjects();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

class CSinglePlayScene : public CScene
{
public:
	CSinglePlayScene();
	~CSinglePlayScene();

	// 씬에서 마우스와 키보드 메시지를 처리한다.
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList);
	virtual void ReleaseObjects();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

class CMultiSettingScene : public CScene
{
public:
	CMultiSettingScene();
	~CMultiSettingScene();

	// 씬에서 마우스와 키보드 메시지를 처리한다.
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList);
	virtual void ReleaseObjects();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////

class CMultiPlayScene : public CScene
{
public:
	CMultiPlayScene();
	~CMultiPlayScene();

	// 씬에서 마우스와 키보드 메시지를 처리한다.
	virtual bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);
	virtual bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM
		lParam);

	virtual void BuildObjects(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList
		* pd3dCommandList);
	virtual void ReleaseObjects();
};
