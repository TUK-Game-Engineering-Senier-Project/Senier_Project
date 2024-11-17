#pragma once

#include "stdafx.h"
#include "Mesh.h"

// FBX SDK 사용을 위해 import
#include <fbxsdk.h>
#include <vector>

// 적 버텍스
struct CDiffusedEnemyVertex {
    XMFLOAT3 Position;
    XMFLOAT4 Color;
    XMFLOAT2 TexCoord;

    CDiffusedEnemyVertex(const XMFLOAT3& pos, const XMFLOAT4& col, const XMFLOAT2& tex)
        : Position(pos), Color(col), TexCoord(tex) {}
};

class CEnemyMeshDiffused : public CMesh
{
public:
    CEnemyMeshDiffused(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* filename,
        const char* textureFilePath, float sizeX, float sizeY, float sizeZ);
    virtual ~CEnemyMeshDiffused();

    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pd3dSrvDescriptorHeap; // SRV 디스크립터 힙
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pTexture; // 텍스처 리소스
    D3D12_GPU_DESCRIPTOR_HANDLE m_srvHandle; // SRV 핸들

private:
    void LoadFBXMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* filename,
        float sizeX, float sizeY, float sizeZ);
    void ProcessNode(FbxNode* pNode, FbxScene* pScene, std::vector<CDiffusedEnemyVertex>& vertices, std::vector<UINT>& indices,
        float sizeX, float sizeY, float sizeZ);
    void LoadTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* textureFilePath);
};

