#include "Enemy.h"

#include <wincodec.h>
#include "d3dx12.h"
#include <string>

// ### 디버그 테스트용
#include <windows.h>
void DebugMessage(const std::string& message) {
	OutputDebugStringA(message.c_str());
}

// 텍스처 데이터를 파일에서 읽고 D3D12 리소스를 생성하는 함수
// Load texture from file and create a D3D12 resource
ID3D12Resource* LoadTextureFromFile(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
    const char* pszFileName, ID3D12Resource** ppd3dTextureUploadBuffer)
{
    ID3D12Resource* pTexture = nullptr;

    // 1. WIC Factory creation
    IWICImagingFactory* pWICFactory = nullptr;
    CoInitialize(nullptr);
    CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));

    // 2. Load file
    IWICBitmapDecoder* pDecoder = nullptr;
    std::wstring wFileName(pszFileName, pszFileName + strlen(pszFileName));
    pWICFactory->CreateDecoderFromFilename(wFileName.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);

    IWICBitmapFrameDecode* pFrame = nullptr;
    pDecoder->GetFrame(0, &pFrame);

    IWICFormatConverter* pConverter = nullptr;
    pWICFactory->CreateFormatConverter(&pConverter);
    pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);

    UINT width, height;
    pFrame->GetSize(&width, &height);

    // 3. Create texture data buffer
    UINT rowPitch = width * 4; // 4 bytes per pixel (RGBA)
    UINT imageSize = rowPitch * height;
    BYTE* pData = new BYTE[imageSize];
    pConverter->CopyPixels(nullptr, rowPitch, imageSize, pData);

    // 4. Create DirectX 12 resource
    D3D12_RESOURCE_DESC textureDesc = {};
    textureDesc.MipLevels = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    textureDesc.DepthOrArraySize = 1;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

    // Use variables for heap properties and resource desc
    CD3DX12_HEAP_PROPERTIES heapPropertiesDefault(D3D12_HEAP_TYPE_DEFAULT);
    pd3dDevice->CreateCommittedResource(
        &heapPropertiesDefault,
        D3D12_HEAP_FLAG_NONE,
        &textureDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&pTexture));

    UINT64 uploadBufferSize = GetRequiredIntermediateSize(pTexture, 0, 1);

    CD3DX12_HEAP_PROPERTIES heapPropertiesUpload(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC uploadBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

    pd3dDevice->CreateCommittedResource(
        &heapPropertiesUpload,
        D3D12_HEAP_FLAG_NONE,
        &uploadBufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(ppd3dTextureUploadBuffer));

    // 5. Upload texture data
    D3D12_SUBRESOURCE_DATA textureData = {};
    textureData.pData = pData;
    textureData.RowPitch = rowPitch;
    textureData.SlicePitch = textureData.RowPitch * height;

    UpdateSubresources(pd3dCommandList, pTexture, *ppd3dTextureUploadBuffer, 0, 0, 1, &textureData);

    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        pTexture, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

    pd3dCommandList->ResourceBarrier(1, &barrier);

    // 6. Cleanup
    delete[] pData;
    pConverter->Release();
    pFrame->Release();
    pDecoder->Release();
    pWICFactory->Release();

    return pTexture;
}

// ----- CEnemyMeshDiffused 클래스 -----

// 생성자
CEnemyMeshDiffused::CEnemyMeshDiffused(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, 
	const char* filename, const char* partname,
	float sizeX, float sizeY, float sizeZ) : CMesh(pd3dDevice, pd3dCommandList)
{
	// FBX 파일에서 메시 불러오기
	LoadFBXMesh(pd3dDevice, pd3dCommandList, filename, partname, sizeX, sizeY, sizeZ);
}

// 소멸자
CEnemyMeshDiffused::~CEnemyMeshDiffused() { }

// 노드 처리 및 메시 데이터 추출 함수
// ### fileName과 partName을 가지고 fbx 파일 안의 부분 fbx를 가져온다.
// ### ProcessNode에 부분 FBX 이름 확인 코드 있음

void CEnemyMeshDiffused::LoadFBXMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList,
    const char* fileName, const char* partName,
    float sizeX, float sizeY, float sizeZ)
{
    // FBX 매니저
    FbxManager* pManager = FbxManager::Create();
    FbxIOSettings* pIOSettings = FbxIOSettings::Create(pManager, IOSROOT);
    pManager->SetIOSettings(pIOSettings);

    // FBX 씬, FBX 임포터
    FbxScene* pScene = FbxScene::Create(pManager, "Scene");
    FbxImporter* pImporter = FbxImporter::Create(pManager, "");
    bool bImportStatus = pImporter->Initialize(fileName, -1, pManager->GetIOSettings());

    // FBX 파일 임포트 에러시 종료
    if (!bImportStatus) { DebugMessage("Error: loading FBX file"); return; }

    pImporter->Import(pScene);
    pImporter->Destroy();

    // 텍스쳐 파일 좌표 지정
    std::string textureFilePath = "EarthGolem_albedo.jpeg";

    // 텍스쳐 불러오기
    m_pd3dTexture = LoadTextureFromFile(pd3dDevice, pd3dCommandList, textureFilePath.c_str(), &m_pd3dTextureUploadBuffer);

    // 텍스쳐 불러오기 성공 여부 확인
    if (m_pd3dTexture == nullptr) { DebugMessage("Failed to load texture.\n"); }
    else { DebugMessage("Texture [" + textureFilePath + "] loaded successfully.\n"); }

    // 버텍스, 인덱스 데이터
    std::vector<CEnemyDiffusedVertex> vertices;
    std::vector<UINT> indices;

    FbxNode* pRootNode = pScene->GetRootNode();
    if (pRootNode) {
        ProcessNode(pRootNode, pScene, vertices, indices, partName, sizeX, sizeY, sizeZ);
    }

    // 버텍스, 인덱스 데이터 버퍼
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

    // FBX 매니저 정리
    pManager->Destroy();
}

// 노드 처리
void CEnemyMeshDiffused::ProcessNode(FbxNode* pNode, FbxScene* pScene,
    std::vector<CEnemyDiffusedVertex>& vertices, std::vector<UINT>& indices,
    const char* partName,
	float sizeX, float sizeY, float sizeZ)
{
	// ### 여기 코드로 현재 가져오는 부분 노드 이름을 확인
	std::string nodeName = pNode->GetName();
	DebugMessage("### Now node name is: " + nodeName + "\n");

	// ### 현재 부분 노드에 실제 데이터가 있는지 확인
	if (!(pNode->GetNodeAttribute() && pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh))
	{
		DebugMessage("### " + nodeName + " contains nothing\n");
	}

	// 데이터가 있고 이름이 일치하는 부분 노드 처리
	if (pNode->GetNodeAttribute() && pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh
		&& partName == nodeName)
	{
		DebugMessage("### Processing node name is: " + nodeName + "\n");

		FbxMesh* pMesh = (FbxMesh*)pNode->GetNodeAttribute();

		// Extract vertices
		int numVertices = pMesh->GetControlPointsCount();
		for (int i = 0; i < numVertices; ++i) {
			FbxVector4 vertex = pMesh->GetControlPointAt(i);

			// Scale vertex positions
			XMFLOAT3 scaledVertex(
				static_cast<float>(vertex[0] * sizeX),
				static_cast<float>(vertex[1] * sizeY),
				static_cast<float>(vertex[2] * sizeZ)
			);

			// Default texture coordinates (0, 0) in case no mapping exists
			XMFLOAT2 defaultTexCoord(0.0f, 0.0f);

			vertices.push_back(CEnemyDiffusedVertex(scaledVertex, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), defaultTexCoord));
		}

		// Extract indices and texture coordinates
		FbxLayerElementUV* pUVLayer = pMesh->GetElementUV();
		
        if (pUVLayer) {
            FbxLayerElement::EMappingMode mappingMode = pUVLayer->GetMappingMode();
            FbxLayerElement::EReferenceMode refMode = pUVLayer->GetReferenceMode();
            const auto& uvArray = pUVLayer->GetDirectArray();

            // Loop over polygons
            for (int i = 0; i < pMesh->GetPolygonCount(); ++i) {
                int numIndices = pMesh->GetPolygonSize(i);

                for (int j = 0; j < numIndices; ++j) {
                    int ctrlPointIndex = pMesh->GetPolygonVertex(i, j);
                    indices.push_back(ctrlPointIndex);

                    if (pUVLayer && mappingMode == FbxLayerElement::eByPolygonVertex) {
                        int uvIndex = (refMode == FbxLayerElement::eIndexToDirect)
                            ? pUVLayer->GetIndexArray().GetAt(i * numIndices + j)
                            : ctrlPointIndex;

                        const FbxVector2& uv = uvArray[uvIndex];
                        vertices[ctrlPointIndex].TexCoord = XMFLOAT2(static_cast<float>(uv[0]), 1.0f - static_cast<float>(uv[1]));
                    }
                }
            }
        }
	}

	// 하위 노드 처리
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		ProcessNode(pNode->GetChild(i), pScene, vertices, indices, partName, sizeX, sizeY, sizeZ);
	}
}