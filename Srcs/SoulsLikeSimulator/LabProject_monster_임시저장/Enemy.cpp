#include "Enemy.h"

// ### 디버그 테스트용
#include <windows.h>
void DebugMessage(const std::string& message) {
	OutputDebugStringA(message.c_str());
}

// ----- CEnemyMeshDiffused 클래스 -----

// 생성자
CEnemyMeshDiffused::CEnemyMeshDiffused(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* filename,
	float sizeX, float sizeY, float sizeZ) : CMesh(pd3dDevice, pd3dCommandList)
{
	// FBX 파일에서 메시 불러오기
	LoadFBXMesh(pd3dDevice, pd3dCommandList, filename, sizeX, sizeY, sizeZ);
}

// 소멸자
CEnemyMeshDiffused::~CEnemyMeshDiffused() { }

// 노드 처리 및 메시 데이터 추출 함수
void CEnemyMeshDiffused::LoadFBXMesh(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, const char* filename, float sizeX, float sizeY, float sizeZ)
{
	FbxManager* pManager = FbxManager::Create();  // Create the FBX Manager
	FbxIOSettings* pIOSettings = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(pIOSettings);

	// FBX 씬 생성
	FbxScene* pScene = FbxScene::Create(pManager, "Scene");

	// 파일 임포터 생성
	FbxImporter* pImporter = FbxImporter::Create(pManager, "");
	bool bImportStatus = pImporter->Initialize(filename, -1, pManager->GetIOSettings());

	if (!bImportStatus)
	{
		DebugMessage("Error loading FBX file");
		return;
	}

	pImporter->Import(pScene);
	pImporter->Destroy();

	std::vector<CDiffusedVertex> vertices;
	std::vector<UINT> indices;

	// 씬 노드 처리
	FbxNode* pRootNode = pScene->GetRootNode();

	// 메시 데이터 추출
	if (pRootNode) ProcessNode(pRootNode, pScene, vertices, indices, sizeX, sizeY, sizeZ);

	// 추출된 버텍스와 인덱스를 버퍼로 변환
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

	// FBX SDK 리소스 정리
	pManager->Destroy();
}

// 노드 처리
void CEnemyMeshDiffused::ProcessNode(FbxNode* pNode, FbxScene* pScene, std::vector<CDiffusedVertex>& vertices, std::vector<UINT>& indices,
	float sizeX, float sizeY, float sizeZ)
{
	if (pNode->GetNodeAttribute() && pNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		FbxMesh* pMesh = (FbxMesh*)pNode->GetNodeAttribute();

		// 메시 버텍스 처리
		int numVertices = pMesh->GetControlPointsCount();
		for (int i = 0; i < numVertices; ++i)
		{
			FbxVector4 vertex = pMesh->GetControlPointAt(i);

			// 크기 조정
			XMFLOAT3 scaledVertex(
				static_cast<float>(vertex[0] * sizeX),
				static_cast<float>(vertex[1] * sizeY),
				static_cast<float>(vertex[2] * sizeZ)
			);

			// 크기 조정된 버텍스 적용
			vertices.push_back(CDiffusedVertex(scaledVertex, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)));
		}

		// 메시 표면 처리
		for (int i = 0; i < pMesh->GetPolygonCount(); ++i)
		{
			int numIndices = pMesh->GetPolygonSize(i);
			for (int j = 0; j < numIndices; ++j)
			{
				indices.push_back(pMesh->GetPolygonVertex(i, j));
			}
		}
	}

	// 하위 노드 처리
	for (int i = 0; i < pNode->GetChildCount(); ++i)
	{
		ProcessNode(pNode->GetChild(i), pScene, vertices, indices, sizeX, sizeY, sizeZ);
	}
}