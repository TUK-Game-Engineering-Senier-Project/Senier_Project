#pragma once

#include "stdafx.h"
#include "Mesh.h"

// FBX SDK 사용을 위해 import
#include <fbxsdk.h>
#include <vector>

class CEnemyMeshDiffused : public CMesh
{
public:

    CEnemyMeshDiffused(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
        const char* filename, const char* partname,
        float sizeX, float sizeY, float sizeZ);

        virtual ~CEnemyMeshDiffused();

private:
    void LoadFBXMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
        const char* fileName, const char* partName,
        float sizeX, float sizeY, float sizeZ);

    void ProcessNode(FbxNode* pNode, FbxScene* pScene,
        std::vector<CDiffusedVertex>& vertices, std::vector<UINT>& indices,
        const char* partName,
        float sizeX, float sizeY, float sizeZ);
};