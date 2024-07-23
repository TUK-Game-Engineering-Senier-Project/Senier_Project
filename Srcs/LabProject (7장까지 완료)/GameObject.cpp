#include "stdafx.h"
#include "GameObject.h"
#include "Shader.h"

// ������
CGameObject::CGameObject()
{
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixIdentity());
}

// �Ҹ���
CGameObject::~CGameObject()
{
	if (m_pMesh) m_pMesh->Release();
	if (m_pShader)
	{
		m_pShader->ReleaseShaderVariables();
		m_pShader->Release();
	}
}

// SetShader()
void CGameObject::SetShader(CShader* pShader)
{
	if (m_pShader) m_pShader->Release();
	m_pShader = pShader;
	if (m_pShader) m_pShader->AddRef();
}

// SetMesh()
void CGameObject::SetMesh(CMesh* pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}

// ReleaseUploadBuffers()
void CGameObject::ReleaseUploadBuffers()
{
	// ���� ���۸� ���� ���ε� ���۸� �Ҹ��Ų��.
	if (m_pMesh) m_pMesh->ReleaseUploadBuffers();
}

// Animate()
void CGameObject::Animate(float fTimeElapsed)
{
}

// OnPrepareRender()
void CGameObject::OnPrepareRender()
{
}

// Render()
void CGameObject::Render(ID3D12GraphicsCommandList * pd3dCommandList)
{
	OnPrepareRender();
	//���� ��ü�� ���̴� ��ü�� ����Ǿ� ������ ���̴� ���� ��ü�� �����Ѵ�.
	if (m_pShader) m_pShader->Render(pd3dCommandList);
	//���� ��ü�� �޽��� ����Ǿ� ������ �޽��� �������Ѵ�.
	if (m_pMesh) m_pMesh->Render(pd3dCommandList);
}