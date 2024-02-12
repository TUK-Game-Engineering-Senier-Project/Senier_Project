#include "Objects.h"

// ----- 오브젝트 관련 내용 cpp 파일 -----

SoulSimul::SoulSimul(HINSTANCE hInstance)
    : GameApp(hInstance)
{
}

SoulSimul::~SoulSimul()
{
}

bool SoulSimul::Initialize()
{
    if (!GameApp::Initialize())
        return false;

    // Reset the command list to prep for initialization commands.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

    BuildDescriptorHeaps();
    BuildConstantBuffers();
    BuildRootSignature();
    BuildShadersAndInputLayout();
    
    BuildPlayerGeometry(); // 플레이어
    BuildPlayerLookGeometry(); // 플레이어 시선

    BuildPSO();

    // Execute the initialization commands.
    ThrowIfFailed(mCommandList->Close());
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until initialization is complete.
    FlushCommandQueue();

    return true;
}

void SoulSimul::OnResize()
{
    GameApp::OnResize();

    // The window resized, so update the aspect ratio and recompute the projection matrix.
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
    XMStoreFloat4x4(&mProj, P);
}

void SoulSimul::Update(const GameTimer& gt)
{
    // Convert Spherical to Cartesian coordinates.
    float x = mRadius * sinf(mPhi) * cosf(mTheta);
    float z = mRadius * sinf(mPhi) * sinf(mTheta);
    float y = mRadius * cosf(mPhi);

    // Build the view matrix.
    XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
    XMStoreFloat4x4(&mView, view);

    // 월드를 플레이어 위치에 맞춰 변환
    // 여기에 player[g_id].trans_x (y, z)가 적용되게 함 - 완료
    XMMATRIX translation = XMMatrixTranslation
    (
        player[g_id].trans_x, player[g_id].trans_y, player[g_id].trans_z
    );
    XMMATRIX proj = XMLoadFloat4x4(&mProj);
    XMMATRIX playerViewProj = translation * view * proj;
    ObjectConstants objPlayerConstants;
    XMStoreFloat4x4(&objPlayerConstants.WorldViewProj, XMMatrixTranspose(playerViewProj));

    mPlayerCB->CopyData(0, objPlayerConstants); // 상수 데이터 복사

    // 플레이어 시선 
    XMMATRIX playerLook = XMMatrixTranslation
    (
        2.0f, 0.0f, 0.0f
    );
    XMMATRIX playerLookViewProj = playerViewProj * playerLook;
    ObjectConstants objPlayerLookConstants;
    XMStoreFloat4x4(&objPlayerLookConstants.WorldViewProj, XMMatrixTranspose(playerLookViewProj));
    
    // mPlayerLookCB->CopyData(0, objPlayerLookConstants); // 상수 데이터 복사
}

void SoulSimul::Draw(const GameTimer& gt)
{
    // Reuse the memory associated with command recording.
    // We can only reset when the associated command lists have finished execution on the GPU.
    ThrowIfFailed(mDirectCmdListAlloc->Reset());

    // A command list can be reset after it has been added to the command queue via ExecuteCommandList.
    // Reusing the command list reuses memory.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), mPSO.Get()));

    mCommandList->RSSetViewports(1, &mScreenViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    // Indicate a state transition on the resource usage.
    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    // Clear the back buffer and depth buffer.
    mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
    mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    // Specify the buffers we are going to render to.
    mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

    mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

    ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap.Get() };
    mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

    // 플레이어 그리기

    mCommandList->IASetVertexBuffers(0, 1, &mPlayer->VertexBufferView());
    mCommandList->IASetIndexBuffer(&mPlayer->IndexBufferView());
    mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());
    mCommandList->DrawIndexedInstanced(mPlayer->DrawArgs["player"].IndexCount, 1, 0, 0, 0);

    // 플레이어 바라보는 방향 그리기

    mCommandList->IASetVertexBuffers(0, 1, &mPlayerLook->VertexBufferView());
    mCommandList->IASetIndexBuffer(&mPlayerLook->IndexBufferView());
    mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());
    mCommandList->DrawIndexedInstanced(mPlayerLook->DrawArgs["playerLook"].IndexCount, 1, 0, 0, 0);

    // Indicate a state transition on the resource usage.
    mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
        D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    // Done recording commands.
    ThrowIfFailed(mCommandList->Close());

    // Add the command list to the queue for execution.
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // swap the back and front buffers
    ThrowIfFailed(mSwapChain->Present(0, 0));
    mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

    // Wait until frame commands are complete.  This waiting is inefficient and is
    // done for simplicity.  Later we will show how to organize our rendering code
    // so we do not have to wait per frame.
    FlushCommandQueue();
}

void SoulSimul::OnMouseDown(WPARAM btnState, int x, int y)
{
    mLastMousePos.x = x;
    mLastMousePos.y = y;

    SetCapture(mhMainWnd);
}

void SoulSimul::OnMouseUp(WPARAM btnState, int x, int y)
{
    ReleaseCapture();
}

void SoulSimul::OnMouseMove(WPARAM btnState, int x, int y)
{
    if ((btnState & MK_LBUTTON) != 0)
    {
        // Make each pixel correspond to a quarter of a degree.
        float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
        float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

        // Update angles based on input to orbit camera around box.
        mTheta -= dx;
        mPhi += dy;

        // 아래에서는 안 보이게 각도 제한
        mPhi = MathHelper::Clamp(mPhi, 0.5f, MathHelper::Pi - 2.0f);
    }
    else if ((btnState & MK_RBUTTON) != 0)
    {
        // Make each pixel correspond to 0.005 unit in the scene.
        float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
        float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);

        // Update the camera radius based on input.
        mRadius += dx - dy;

        // Restrict the radius.
        mRadius = MathHelper::Clamp(mRadius, 6.0f, 15.0f);
    }

    mLastMousePos.x = x;
    mLastMousePos.y = y;
}

void SoulSimul::BuildDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
    cbvHeapDesc.NumDescriptors = 1;
    cbvHeapDesc.NodeMask = 0;
    cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc2;
    cbvHeapDesc2.NumDescriptors = 2;
    cbvHeapDesc2.NodeMask = 1;
    cbvHeapDesc2.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvHeapDesc2.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap)));
    //ThrowIfFailed(md3dDevice2->CreateDescriptorHeap(&cbvHeapDesc2, IID_PPV_ARGS(&mCbvHeap2)));
}

void SoulSimul::BuildConstantBuffers()
{
    mPlayerCB = std::make_unique<UploadBuffer<ObjectConstants>>(md3dDevice.Get(), 1, true);
    //mPlayerLookCB = std::make_unique<UploadBuffer<ObjectConstants>>(md3dDevice2.Get(), 1, true);

    UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));

    int boxCBufIndex = 0;

    // 플레이어

    D3D12_GPU_VIRTUAL_ADDRESS cbPlayerAddress = mPlayerCB->Resource()->GetGPUVirtualAddress();
    // Offset to the ith object constant buffer in the buffer.
    
    cbPlayerAddress += boxCBufIndex * objCBByteSize;

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvPlayerDesc;
    cbvPlayerDesc.BufferLocation = cbPlayerAddress;
    cbvPlayerDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
    md3dDevice->CreateConstantBufferView(&cbvPlayerDesc, mCbvHeap->GetCPUDescriptorHandleForHeapStart());

    // 플레이어 시선

    /*
    D3D12_GPU_VIRTUAL_ADDRESS cbPlayerLookAddress = mPlayerLookCB->Resource()->GetGPUVirtualAddress();
    // Offset to the ith object constant buffer in the buffer.

    cbPlayerLookAddress += boxCBufIndex * objCBByteSize;

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvPlayerLookDesc;
    cbvPlayerLookDesc.BufferLocation = cbPlayerLookAddress;
    cbvPlayerLookDesc.SizeInBytes = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
    md3dDevice2->CreateConstantBufferView(&cbvPlayerLookDesc, mCbvHeap2->GetCPUDescriptorHandleForHeapStart());
    */
}

void SoulSimul::BuildRootSignature()
{
    // Shader programs typically require resources as input (constant buffers,
    // textures, samplers).  The root signature defines the resources the shader
    // programs expect.  If we think of the shader programs as a function, and
    // the input resources as function parameters, then the root signature can be
    // thought of as defining the function signature.

    // Root parameter can be a table, root descriptor or root constants.
    CD3DX12_ROOT_PARAMETER slotRootParameter[1];

    // Create a single descriptor table of CBVs.
    CD3DX12_DESCRIPTOR_RANGE cbvTable;
    cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
    slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

    // A root signature is an array of root parameters.
    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    // create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
    ComPtr<ID3DBlob> serializedRootSig = nullptr;
    ComPtr<ID3DBlob> errorBlob = nullptr;
    HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
        serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

    if (errorBlob != nullptr)
    {
        ::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    }
    ThrowIfFailed(hr);

    ThrowIfFailed(md3dDevice->CreateRootSignature(
        0,
        serializedRootSig->GetBufferPointer(),
        serializedRootSig->GetBufferSize(),
        IID_PPV_ARGS(&mRootSignature)));

    /*
    ThrowIfFailed(md3dDevice2->CreateRootSignature(
        0,
        serializedRootSig->GetBufferPointer(),
        serializedRootSig->GetBufferSize(),
        IID_PPV_ARGS(&mRootSignature)));
    */
}

void SoulSimul::BuildShadersAndInputLayout()
{
    HRESULT hr = S_OK;

    mvsByteCode = d3dUtil::CompileShader(L"Shaders\\color.hlsl", nullptr, "VS", "vs_5_0");
    mpsByteCode = d3dUtil::CompileShader(L"Shaders\\color.hlsl", nullptr, "PS", "ps_5_0");

    mInputLayout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };
}

void SoulSimul::BuildPSO()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
    psoDesc.pRootSignature = mRootSignature.Get();
    psoDesc.VS =
    {
        reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
        mvsByteCode->GetBufferSize()
    };
    psoDesc.PS =
    {
        reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
        mpsByteCode->GetBufferSize()
    };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = mBackBufferFormat;
    psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
    psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
    psoDesc.DSVFormat = mDepthStencilFormat;

    ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
    // ThrowIfFailed(md3dDevice2->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO)));
}

void SoulSimul::BuildPlayerGeometry()
{
    // 여기도 scale_x (y, z)로 수정함
    float fxScale = player[g_id].scale_x;
    float fyScale = player[g_id].scale_y;
    float fzScale = player[g_id].scale_z;

    std::array<Vertex, 8> vertices =
    {
        Vertex({ XMFLOAT3(-fxScale, -fyScale, -fzScale), XMFLOAT4(Colors::White) }),
        Vertex({ XMFLOAT3(-fxScale, +fyScale, -fzScale), XMFLOAT4(Colors::Black) }),
        Vertex({ XMFLOAT3(+fxScale, +fyScale, -fzScale), XMFLOAT4(Colors::Red) }),
        Vertex({ XMFLOAT3(+fxScale, -fyScale, -fzScale), XMFLOAT4(Colors::Green) }),
        Vertex({ XMFLOAT3(-fxScale, -fyScale, +fzScale), XMFLOAT4(Colors::Blue) }),
        Vertex({ XMFLOAT3(-fxScale, +fyScale, +fzScale), XMFLOAT4(Colors::Yellow) }),
        Vertex({ XMFLOAT3(+fxScale, +fyScale, +fzScale), XMFLOAT4(Colors::Cyan) }),
        Vertex({ XMFLOAT3(+fxScale, -fyScale, +fzScale), XMFLOAT4(Colors::Magenta) })
    };

    std::array<std::uint16_t, 36> indices =
    {
        // front face
        0, 1, 2,
        0, 2, 3,

        // back face
        4, 6, 5,
        4, 7, 6,

        // left face
        4, 5, 1,
        4, 1, 0,

        // right face
        3, 2, 6,
        3, 6, 7,

        // top face
        1, 5, 6,
        1, 6, 2,

        // bottom face
        4, 0, 3,
        4, 3, 7
    };

    const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
    const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

    mPlayer = std::make_unique<MeshGeometry>();
    mPlayer->Name = "playerGeo";

    ThrowIfFailed(D3DCreateBlob(vbByteSize, &mPlayer->VertexBufferCPU));
    CopyMemory(mPlayer->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

    ThrowIfFailed(D3DCreateBlob(ibByteSize, &mPlayer->IndexBufferCPU));
    CopyMemory(mPlayer->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

    mPlayer->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
        mCommandList.Get(), vertices.data(), vbByteSize, mPlayer->VertexBufferUploader);

    mPlayer->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
        mCommandList.Get(), indices.data(), ibByteSize, mPlayer->IndexBufferUploader);

    mPlayer->VertexByteStride = sizeof(Vertex);
    mPlayer->VertexBufferByteSize = vbByteSize;
    mPlayer->IndexFormat = DXGI_FORMAT_R16_UINT;
    mPlayer->IndexBufferByteSize = ibByteSize;

    SubmeshGeometry submesh;
    submesh.IndexCount = (UINT)indices.size();
    submesh.StartIndexLocation = 0;
    submesh.BaseVertexLocation = 0;

    mPlayer->DrawArgs["player"] = submesh;
}

void SoulSimul::BuildPlayerLookGeometry()
{
    float fxScale = 0.2f;
    float fyScale = 0.2f;
    float fzScale = 0.2f;

    std::array<Vertex, 8> vertices =
    {
        Vertex({ XMFLOAT3(-fxScale, -fyScale, -fzScale), XMFLOAT4(Colors::White) }),
        Vertex({ XMFLOAT3(-fxScale, +fyScale, -fzScale), XMFLOAT4(Colors::Black) }),
        Vertex({ XMFLOAT3(+fxScale, +fyScale, -fzScale), XMFLOAT4(Colors::White) }),
        Vertex({ XMFLOAT3(+fxScale, -fyScale, -fzScale), XMFLOAT4(Colors::Black) }),
        Vertex({ XMFLOAT3(-fxScale, -fyScale, +fzScale), XMFLOAT4(Colors::White) }),
        Vertex({ XMFLOAT3(-fxScale, +fyScale, +fzScale), XMFLOAT4(Colors::Black) }),
        Vertex({ XMFLOAT3(+fxScale, +fyScale, +fzScale), XMFLOAT4(Colors::White) }),
        Vertex({ XMFLOAT3(+fxScale, -fyScale, +fzScale), XMFLOAT4(Colors::Black) })
    };

    std::array<std::uint16_t, 36> indices =
    {
        // front face
        0, 1, 2,
        0, 2, 3,

        // back face
        4, 6, 5,
        4, 7, 6,

        // left face
        4, 5, 1,
        4, 1, 0,

        // right face
        3, 2, 6,
        3, 6, 7,

        // top face
        1, 5, 6,
        1, 6, 2,

        // bottom face
        4, 0, 3,
        4, 3, 7
    };

    const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
    const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);

    mPlayerLook = std::make_unique<MeshGeometry>();
    mPlayerLook->Name = "playerLookGeo";

    ThrowIfFailed(D3DCreateBlob(vbByteSize, &mPlayerLook->VertexBufferCPU));
    CopyMemory(mPlayerLook->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

    ThrowIfFailed(D3DCreateBlob(ibByteSize, &mPlayerLook->IndexBufferCPU));
    CopyMemory(mPlayerLook->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

    mPlayerLook->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
        mCommandList.Get(), vertices.data(), vbByteSize, mPlayerLook->VertexBufferUploader);

    mPlayerLook->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
        mCommandList.Get(), indices.data(), ibByteSize, mPlayerLook->IndexBufferUploader);

    mPlayerLook->VertexByteStride = sizeof(Vertex);
    mPlayerLook->VertexBufferByteSize = vbByteSize;
    mPlayerLook->IndexFormat = DXGI_FORMAT_R16_UINT;
    mPlayerLook->IndexBufferByteSize = ibByteSize;

    SubmeshGeometry submesh;
    submesh.IndexCount = (UINT)indices.size();
    submesh.StartIndexLocation = 0;
    submesh.BaseVertexLocation = 0;

    mPlayerLook->DrawArgs["playerLook"] = submesh;
}