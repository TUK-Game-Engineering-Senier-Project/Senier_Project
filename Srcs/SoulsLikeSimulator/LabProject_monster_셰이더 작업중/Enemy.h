#pragma once

#include "stdafx.h"
#include "Mesh.h"

// FBX SDK 사용을 위해 import
#include <fbxsdk.h>
#include <vector>

struct CEnemyDiffusedVertex
{
    XMFLOAT3 Position;   // Vertex position
    XMFLOAT4 Color;      // Vertex color
    XMFLOAT2 TexCoord;   // Texture coordinates

    // Constructor
    CEnemyDiffusedVertex(XMFLOAT3 pos, XMFLOAT4 color, XMFLOAT2 texCoord)
        : Position(pos), Color(color), TexCoord(texCoord) {}
};

class CEnemyMeshDiffused : public CMesh
{
public:

    // 생성자
    CEnemyMeshDiffused(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
        const char* filename, const char* partname,
        float sizeX, float sizeY, float sizeZ);

    // 소멸자
    virtual ~CEnemyMeshDiffused();

    // 텍스쳐
    ComPtr<ID3D12Resource> m_pd3dTexture;
    ComPtr<ID3D12Resource> m_pd3dTextureUploadBuffer;

private:

    // FBX 불러오기
    void LoadFBXMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
        const char* fileName, const char* partName,
        float sizeX, float sizeY, float sizeZ);

    // 노드 처리
    void ProcessNode(FbxNode* pNode, FbxScene* pScene,
        std::vector<CEnemyDiffusedVertex>& vertices, std::vector<UINT>& indices,
        const char* partName,
        float sizeX, float sizeY, float sizeZ);
};