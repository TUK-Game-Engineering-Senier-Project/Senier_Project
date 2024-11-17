// C/C++ - 코드 생성 - 다중 스레드 디버그 (/MTd)

// DirectXTex.lib 경로 추가 : 링커 - 일반 - 추가 라이브러리 디렉터리
// D:\DirectXTex - oct2024\DirectXTex\Bin\Desktop_2022\x64\Debug
// C/C++ - 일반 - 추가 포함 경로
// D:\DirectXTex-oct2024\DirectXTex
// 링커 - 입력 - 추가 종속성
// DirectXTex.lib / libfbxsdk - mt.lib / libxml2 - mt.lib / zlib - mt.lib
 

#include <DirectXTex.h>
#include <windows.h>
#include <wrl.h>
#include "d3dx12.h"
#include "Enemy.h"

using namespace Microsoft::WRL;

// Debug message helper
void DebugMessage(const std::string& message) {
    OutputDebugStringA(message.c_str());
}

ID3D12Device* pd3dDevice = nullptr;
ID3D12GraphicsCommandList* pd3dCommandList = nullptr;

// 생성자
CEnemyMeshDiffused::CEnemyMeshDiffused(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* filename,
    const char* textureFilePath, float sizeX, float sizeY, float sizeZ)
    : CMesh(pd3dDevice, pd3dCommandList) {

    // FBX 메시 불러오기
    LoadFBXMesh(pd3dDevice, pd3dCommandList, filename, sizeX, sizeY, sizeZ);

    // 텍스쳐 불러오기
    LoadTexture(pd3dDevice, pd3dCommandList, textureFilePath);
}

// 소멸자
CEnemyMeshDiffused::~CEnemyMeshDiffused() {
}

// FBX 메시 불러오기
void CEnemyMeshDiffused::LoadFBXMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* filename,
    float sizeX, float sizeY, float sizeZ) {
    FbxManager* pManager = FbxManager::Create();
    FbxIOSettings* pIOSettings = FbxIOSettings::Create(pManager, IOSROOT);
    pManager->SetIOSettings(pIOSettings);

    FbxScene* pScene = FbxScene::Create(pManager, "Scene");
    FbxImporter* pImporter = FbxImporter::Create(pManager, "");

    if (!pImporter->Initialize(filename, -1, pManager->GetIOSettings())) {
        DebugMessage("Error loading FBX file");
        return;
    }

    pImporter->Import(pScene);
    pImporter->Destroy();

    std::vector<CDiffusedEnemyVertex> vertices;
    std::vector<UINT> indices;

    FbxNode* pRootNode = pScene->GetRootNode();
    if (pRootNode) {
        ProcessNode(pRootNode, pScene, vertices, indices, sizeX, sizeY, sizeZ);
    }

    m_nVertices = vertices.size();
    m_nIndices = indices.size();

    m_pVertices = new CDiffusedVertex[m_nVertices];
    m_pnIndices = new UINT[m_nIndices];

    memcpy(m_pVertices, vertices.data(), m_nVertices * sizeof(CDiffusedVertex));
    memcpy(m_pnIndices, indices.data(), m_nIndices * sizeof(UINT));

    m_nStride = sizeof(CDiffusedVertex);

    // 버텍스 버퍼 생성
    m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pVertices,
        m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);

    m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
    m_d3dVertexBufferView.StrideInBytes = m_nStride;
    m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;

    // 인덱스 버퍼 생성
    m_pd3dIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pnIndices,
        sizeof(UINT) * m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_pd3dIndexUploadBuffer);

    m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
    m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
    m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;

    pManager->Destroy();
}

// FBX 노드 처리
void CEnemyMeshDiffused::ProcessNode(FbxNode* pNode, FbxScene* pScene, std::vector<CDiffusedEnemyVertex>& vertices, std::vector<UINT>& indices,
    float sizeX, float sizeY, float sizeZ) {
    if (pNode->GetNodeAttribute() && pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
        FbxMesh* pMesh = static_cast<FbxMesh*>(pNode->GetNodeAttribute());

        // 버텍스 처리
        int numVertices = pMesh->GetControlPointsCount();
        for (int i = 0; i < numVertices; ++i) {
            FbxVector4 vertex = pMesh->GetControlPointAt(i);
            XMFLOAT3 scaledVertex(static_cast<float>(vertex[0] * sizeX), static_cast<float>(vertex[1] * sizeY), static_cast<float>(vertex[2] * sizeZ));
            // Add vertex to vertices array
            vertices.push_back(CDiffusedEnemyVertex(scaledVertex, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)));
        }

        // 인덱스 처리
        for (int i = 0; i < pMesh->GetPolygonCount(); ++i) {
            int numIndices = pMesh->GetPolygonSize(i);
            for (int j = 0; j < numIndices; ++j) {
                indices.push_back(pMesh->GetPolygonVertex(i, j));
            }
        }

        // 텍스쳐 불러오기
        const char* texture_dir = "C:/Users/Seungwan/Desktop/LabProject_monster_Workingnow/Textures/lambert1_normal.dds";
        LoadTexture(pd3dDevice, pd3dCommandList, texture_dir);
    }

    // 하위 노드 처리
    for (int i = 0; i < pNode->GetChildCount(); ++i) {
        ProcessNode(pNode->GetChild(i), pScene, vertices, indices, sizeX, sizeY, sizeZ);
    }
}

// 텍스쳐 불러오기
void CEnemyMeshDiffused::LoadTexture(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* textureFilePath) {
    std::wstring wideFilePath(textureFilePath, textureFilePath + strlen(textureFilePath));

    // DirectXTex 초기화
    DirectX::ScratchImage image;
    HRESULT hr = DirectX::LoadFromWICFile(wideFilePath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, image);
    if (FAILED(hr)) {
        DebugMessage(std::string("Loading texture from: ") + textureFilePath);
        DebugMessage("<<Error loading texture file>>");
        return;
    }

    // 불러온 이미지로 2D 텍스쳐 만들기
    const DirectX::Image* img = image.GetImage(0, 0, 0);
    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    textureDesc.Alignment = 0;
    textureDesc.Width = img->width;
    textureDesc.Height = img->height;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.MipLevels = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    // 텍스쳐 리소스 만들기
    CD3DX12_HEAP_PROPERTIES heapProperties_default(D3D12_HEAP_TYPE_DEFAULT);
    hr = pd3dDevice->CreateCommittedResource(
        &heapProperties_default, 
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_pTexture));

    if (FAILED(hr)) {
        DebugMessage("Failed to create texture resource");
        return;
    }

    // GPU에 텍스쳐 데이터 업로드
    D3D12_SUBRESOURCE_DATA textureData = {};
    textureData.pData = img->pixels;
    textureData.RowPitch = static_cast<LONG_PTR>(img->rowPitch);
    textureData.SlicePitch = textureData.RowPitch * img->height;

    // 업로드 힙 만들기
    ComPtr<ID3D12Resource> pUploadHeap;
    CD3DX12_HEAP_PROPERTIES heapProperties_upload(D3D12_HEAP_TYPE_UPLOAD);
    hr = pd3dDevice->CreateCommittedResource(
        &heapProperties_upload, // Pass the address of the heapProperties object
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&m_pTexture));

    if (FAILED(hr)) {
        DebugMessage("Failed to create upload heap");
        return;
    }

    // 업로드 힙에 데이터 복사
    UpdateSubresources(pd3dCommandList, m_pTexture.Get(), pUploadHeap.Get(), 0, 0, 1, &textureData);

    // 셰이더 리소스 뷰 (SRV) 만들기
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    // SRV 디스크립터 추가
    pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), &srvDesc, m_pd3dSrvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

    // SRV 핸들 저장
    m_srvHandle = m_pd3dSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
}