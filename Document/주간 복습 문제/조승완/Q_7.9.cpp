// ---[1�� ����]---

/*
// ShapesApp.cpp 532��° ����
GeometryGenerator::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20); //��
GeometryGenerator::MeshData sphere = geoGen.CreateGeosphere(0.5f, 0); //�� �����Ѵ�

2��° �μ��� ���� ���ڰ� �����Ҽ��� (�ﰢ���� �����ϴ� Ƚ���� ����������)
�� ���� ����� ������ ���������.
*/

// ---[2�� ����]---

/*
// 7.7.4 ��Ʈ CBV (369p~371p) ������ ShapesApp.cpp�� �����ϰ� �����ϸ� �ȴ�.

// void ShapesApp::BuildRootSignature()��
slotRootParameter[0].InitAsConstantBufferView(0);
slotRootParameter[1].InitAsConstantBufferView(1);

// void ShapesApp::Draw(const GameTimer& gt)��
auto passCB = mCurrFrameResource->PassCB->Resource();
mCommandList->SetGraphicsRootConstantBufferView(1, passCB->GetGPUVirtualAddress());

// auto ri = ritems[i]; �Ʒ���
// cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());
// cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
// cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress();
objCBAddress += ri->ObjCBIndex * objCBByteSize;
cmdList->SetGraphicsRootConstantBufferView(0, objCBAddress);
*/

// ---[3�� ����]---

/*
// skull.txt�� �������ϱ� ���� �Լ��� �����
// skull.txt�� ������ ������ 
// ���ؽ�, �ε��� ���� ��� ���� ���� ��
// geo->DrawArgs["skull"] �� �ذ� �׸���� �����Ѵ�

void ShapesApp::BuildSkullGeometry()
{
    std::ifstream inputFile("skull.txt"); // ������ �б������ ����

    if (!inputFile)
    {
        MessageBox(0, L"skull.txt not found.", 0, 0);
        return;
    }

    // �ذ� ���ؽ�
    struct Vertex {
        XMFLOAT3 pos;
        XMFLOAT3 normal;
    };

    UINT vCount = 0; // ���ؽ� ��
    UINT tCount = 0; // �ﰢ�� ��
    std::string none; // ���� �ʴ� ��

    // ���Ͽ��� ���� �о���δ�
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

    inputFile.close(); // ���� �ݱ�

    // �ϳ��� �ε��� ���۷� ����
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

// void ShapesApp::BuildRenderItems()��

// skull�� �������Ѵ�
// ���� �׷��� ������Ʈ�� ���� 22���̱� ������
// 22�� �ε����� (0�� �ε����� ���ԵǹǷ�)
auto skullRitem = std::make_unique<RenderItem>();
skullRitem->ObjCBIndex = 22;
skullRitem->Geo = mGeometries["skullGeo"].get();
skullRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
skullRitem->IndexCount = skullRitem->Geo->DrawArgs["skull"].IndexCount;
skullRitem->StartIndexLocation = skullRitem->Geo->DrawArgs["skull"].StartIndexLocation;
skullRitem->BaseVertexLocation = skullRitem->Geo->DrawArgs["skull"].BaseVertexLocation;
mAllRitems.push_back(std::move(skullRitem));

*/