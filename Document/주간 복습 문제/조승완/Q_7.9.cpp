// ---[1번 문제]---

/*
// ShapesApp.cpp 532번째 줄의
GeometryGenerator::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20); //를
GeometryGenerator::MeshData sphere = geoGen.CreateGeosphere(0.5f, 0); //로 수정한다

2번째 인수로 들어가는 숫자가 증가할수록 (삼각형을 세분하는 횟수가 많아질수록)
더 구에 가까운 도형이 만들어진다.
*/

// ---[2번 문제]---

/*
// 7.7.4 루트 CBV (369p~371p) 과정을 ShapesApp.cpp에 동일하게 수행하면 된다.

// void ShapesApp::BuildRootSignature()에
slotRootParameter[0].InitAsConstantBufferView(0);
slotRootParameter[1].InitAsConstantBufferView(1);

// void ShapesApp::Draw(const GameTimer& gt)에
auto passCB = mCurrFrameResource->PassCB->Resource();
mCommandList->SetGraphicsRootConstantBufferView(1, passCB->GetGPUVirtualAddress());

// auto ri = ritems[i]; 아래에
// cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());
// cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
// cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress();
objCBAddress += ri->ObjCBIndex * objCBByteSize;
cmdList->SetGraphicsRootConstantBufferView(0, objCBAddress);
*/

// ---[3번 문제]---

/*
// skull.txt를 렌더링하기 위한 함수를 만든다
// skull.txt의 내용을 가져와 
// 버텍스, 인덱스 연결 등등 과정 수행 후
// geo->DrawArgs["skull"] 을 해골 그리기로 지정한다

void ShapesApp::BuildSkullGeometry()
{
    std::ifstream inputFile("skull.txt"); // 파일을 읽기용으로 연다

    if (!inputFile)
    {
        MessageBox(0, L"skull.txt not found.", 0, 0);
        return;
    }

    // 해골 버텍스
    struct Vertex {
        XMFLOAT3 pos;
        XMFLOAT3 normal;
    };

    UINT vCount = 0; // 버텍스 수
    UINT tCount = 0; // 삼각형 수
    std::string none; // 쓰지 않는 값

    // 파일에서 값을 읽어들인다
    inputFile >> none >> vCount >> none >> tCount;
    inputFile >> none >> none >> none >> none;
    std::vector<Vertex> vertices(vCount);
    for (UINT i = 0; i < vCount; ++i)
    {
        inputFile >> vertices[i].pos.x >> vertices[i].pos.y >> vertices[i].pos.z;
        inputFile >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;
    }
    inputFile >> none >> none >> none;

    std::vector<std::int32_t> indices(3 * tCount);
    for (UINT i = 0; i < tCount; ++i)
    {
        inputFile >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
    }

    inputFile.close(); // 파일 닫기

    // 하나의 인덱스 버퍼로 묶기
    const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
    const UINT ibByteSize = (UINT)indices.size() * sizeof(std::int32_t);

    auto geo = std::make_unique<MeshGeometry>();
    geo->Name = "skullGeo";

    ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
    CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
    ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
    CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

    geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
        mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);
    geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
        mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

    geo->VertexByteStride = sizeof(Vertex);
    geo->VertexBufferByteSize = vbByteSize;
    geo->IndexFormat = DXGI_FORMAT_R32_UINT;
    geo->IndexBufferByteSize = ibByteSize;

    SubmeshGeometry submesh;
    submesh.IndexCount = (UINT)indices.size();
    submesh.StartIndexLocation = 0;
    submesh.BaseVertexLocation = 0;

    geo->DrawArgs["skull"] = submesh;

    mGeometries[geo->Name] = std::move(geo);
}

// void ShapesApp::BuildRenderItems()에

// skull을 렌더링한다
// 현재 그려진 오브젝트가 총합 22개이기 때문에
// 22번 인덱스에 (0번 인덱스도 포함되므로)
auto skullRitem = std::make_unique<RenderItem>();
skullRitem->ObjCBIndex = 22;
skullRitem->Geo = mGeometries["skullGeo"].get();
skullRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
skullRitem->IndexCount = skullRitem->Geo->DrawArgs["skull"].IndexCount;
skullRitem->StartIndexLocation = skullRitem->Geo->DrawArgs["skull"].StartIndexLocation;
skullRitem->BaseVertexLocation = skullRitem->Geo->DrawArgs["skull"].BaseVertexLocation;
mAllRitems.push_back(std::move(skullRitem));

*/