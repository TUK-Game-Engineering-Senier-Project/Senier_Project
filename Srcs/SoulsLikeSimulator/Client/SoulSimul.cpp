//***************************************************************************************
// SoulSimul.cpp by Frank Luna (C) 2015 All Rights Reserved.
//***************************************************************************************

#include "../Common/d3dApp.h"
#include "../Common/MathHelper.h"
#include "../Common/UploadBuffer.h"
#include "../Common/GeometryGenerator.h"
#include "FrameResource.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")

const int gNumFrameResources = 3;

// 각 오브젝트 인덱스
constexpr char INDEXPLAYER = 0;
constexpr char INDEXFLOOR = 1;
constexpr char INDEXWEAPON = 2;
constexpr char INDEXSHIELD = 3;

// 각 오브젝트 상대 위치
constexpr float WEAPON_POSFROMPLAYER = 1.3f; // 플레이어 우측 무기
constexpr float SHIELD_POSFROMPLAYER = 1.7f; // 플레이어 좌측 방패
constexpr float WEAPON_UPFROMFLOOR   = 2.0f; // 무기 드는 높이
constexpr float SHIELD_UPFROMFLOOR   = 1.0f; // 방패 드는 높이

// Lightweight structure stores parameters to draw a shape.
// This will vary from app-to-app.
struct RenderItem
{
	RenderItem() = default;

    // World matrix of the shape that describes the object's local space
    // relative to the world space, which defines the position, orientation,
    // and scale of the object in the world.
    XMFLOAT4X4 World = MathHelper::Identity4x4();

	XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();

	// Dirty flag indicating the object data has changed and we need to update the constant buffer.
	// Because we have an object cbuffer for each FrameResource, we have to apply the
	// update to each FrameResource.  Thus, when we modify obect data we should set 
	// NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
	int NumFramesDirty = gNumFrameResources;

	// Index into GPU constant buffer corresponding to the ObjectCB for this render item.
	UINT ObjCBIndex = -1;

	Material* Mat = nullptr;
	MeshGeometry* Geo = nullptr;

    // Primitive topology.
    D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    // DrawIndexedInstanced parameters.
    UINT IndexCount = 0;
    UINT StartIndexLocation = 0;
    int BaseVertexLocation = 0;
};

class SoulSimul : public D3DApp
{
public:
    SoulSimul(HINSTANCE hInstance);
    SoulSimul(const SoulSimul& rhs) = delete;
    SoulSimul& operator=(const SoulSimul& rhs) = delete;
    ~SoulSimul();

    virtual bool Initialize()override;

private:
    virtual void OnResize()override;
    virtual void Update(const GameTimer& gt)override;
    virtual void Draw(const GameTimer& gt)override;

    virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
    virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
    virtual void OnMouseMove(WPARAM btnState, int x, int y)override;

	// 플레이어 업데이트
	void UpdatePlayer();

	// 카메라 업데이트 (플레이어 위치 확정된 뒤)
	void UpdateCamera(const GameTimer& gt);

	void AnimateMaterials(const GameTimer& gt);
	void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMaterialCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);

    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildShapeGeometry();
	void BuildSkullGeometry();
    void BuildPSOs();
    void BuildFrameResources();

	// 재질 build (코드 간략화)
	void BuildMaterial(char* name, int index,
		XMFLOAT4 DiffuseAlbedo, XMFLOAT3 FresnelR0, float roughness);

    void BuildMaterials();

    void BuildRenderItems();
    void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);
 
private:

    std::vector<std::unique_ptr<FrameResource>> mFrameResources;
    FrameResource* mCurrFrameResource = nullptr;
    int mCurrFrameResourceIndex = 0;

    UINT mCbvSrvDescriptorSize = 0;

    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;
	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    ComPtr<ID3D12PipelineState> mOpaquePSO = nullptr;
 
	// List of all the render items.
	std::vector<std::unique_ptr<RenderItem>> mAllRitems;

	// Render items divided by PSO.
	std::vector<RenderItem*> mOpaqueRitems;

    PassConstants mMainPassCB;

	XMFLOAT3 mEyePos = { 0.0f, 0.0f, 0.0f };
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();

    float mTheta = 1.5f*XM_PI;
    float mPhi = 0.2f*XM_PI;
    float mRadius = 10.0f;

    POINT mLastMousePos;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
    PSTR cmdLine, int showCmd)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    try
    {
        SoulSimul theApp(hInstance);
        if(!theApp.Initialize())
            return 0;

        return theApp.Run();
    }
    catch(DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }
}

SoulSimul::SoulSimul(HINSTANCE hInstance)
    : D3DApp(hInstance)
{
}

SoulSimul::~SoulSimul()
{
    if(md3dDevice != nullptr)
        FlushCommandQueue();
}

bool SoulSimul::Initialize()
{
    if(!D3DApp::Initialize())
        return false;

    // Reset the command list to prep for initialization commands.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

    // Get the increment size of a descriptor in this heap type.  This is hardware specific, 
	// so we have to query this information.
    mCbvSrvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    BuildRootSignature();
    BuildShadersAndInputLayout();
    BuildShapeGeometry();
	BuildSkullGeometry();
	BuildMaterials();
    BuildRenderItems();
    BuildFrameResources();
    BuildPSOs();

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
    D3DApp::OnResize();

    // The window resized, so update the aspect ratio and recompute the projection matrix.
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
    XMStoreFloat4x4(&mProj, P);
}

void SoulSimul::Update(const GameTimer& gt)
{
	// 플레이어 업데이트
	UpdatePlayer();

	// 카메라 업데이트 (플레이어 위치 확정된 뒤)
	UpdateCamera(gt);

    // Cycle through the circular frame resource array.
    mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % gNumFrameResources;
    mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();

    // Has the GPU finished processing the commands of the current frame resource?
    // If not, wait until the GPU has completed commands up to this fence point.
    if(mCurrFrameResource->Fence != 0 && mFence->GetCompletedValue() < mCurrFrameResource->Fence)
    {
        HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
        ThrowIfFailed(mFence->SetEventOnCompletion(mCurrFrameResource->Fence, eventHandle));
        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }

	AnimateMaterials(gt);
	UpdateObjectCBs(gt);
	UpdateMaterialCBs(gt);
	UpdateMainPassCB(gt);
}

// 플레이어 업데이트
void SoulSimul::UpdatePlayer()
{
	// 플레이어 각도를 카메라 각도에 맞춰 회전
	// player[g_id].rotate_y -= mTheta + 1.5f;

	// 플레이어 이동
	player[g_id].pos_x += player[g_id].move_x;
	player[g_id].pos_y += player[g_id].move_y;
	player[g_id].pos_z += player[g_id].move_z;

	// 플레이어 이동거리 초기화
	player[g_id].move_x = 0.0f;
	player[g_id].move_y = 0.0f;
	player[g_id].move_z = 0.0f;

	auto skullRitem = std::make_unique<RenderItem>();

	// 플레이어 갱신
	XMStoreFloat4x4
	(
		// 플레이어 배율, 회전, 위치에 맞춰 지정
		&skullRitem->World,
		XMMatrixScaling(player[g_id].scale_x, player[g_id].scale_y, player[g_id].scale_z)
		* XMMatrixRotationRollPitchYaw(player[g_id].rotate_x, player[g_id].rotate_y, player[g_id].rotate_z)
		* XMMatrixTranslation(player[g_id].pos_x, player[g_id].pos_y, player[g_id].pos_z)
	);

	skullRitem->TexTransform = MathHelper::Identity4x4();
	skullRitem->ObjCBIndex = INDEXPLAYER; // 플레이어 인덱스
	skullRitem->Mat = mMaterials["skullMat"].get();
	skullRitem->Geo = mGeometries["skullGeo"].get();
	skullRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	skullRitem->IndexCount = skullRitem->Geo->DrawArgs["skull"].IndexCount;
	skullRitem->StartIndexLocation = skullRitem->Geo->DrawArgs["skull"].StartIndexLocation;
	skullRitem->BaseVertexLocation = skullRitem->Geo->DrawArgs["skull"].BaseVertexLocation;
	mAllRitems.push_back(std::move(skullRitem));

	// 무기

	float weaponX = 0.0f;
	float weaponY = 0.0f;
	float weaponZ = 0.0f;

	// 직선 4방향 (무기)

	if (player[g_id].rotate_y == 0.0f * XM_PI)
	{
		weaponX = player[g_id].pos_x - WEAPON_POSFROMPLAYER;
		weaponY = player[g_id].pos_y + WEAPON_UPFROMFLOOR;
		weaponZ = player[g_id].pos_z;
	}
	if (player[g_id].rotate_y == 0.5f * XM_PI)
	{
		weaponX = player[g_id].pos_x;
		weaponY = player[g_id].pos_y + WEAPON_UPFROMFLOOR;
		weaponZ = player[g_id].pos_z + WEAPON_POSFROMPLAYER;
	}
	if (player[g_id].rotate_y == 1.0f * XM_PI)
	{
		weaponX = player[g_id].pos_x + WEAPON_POSFROMPLAYER;
		weaponY = player[g_id].pos_y + WEAPON_UPFROMFLOOR;
		weaponZ = player[g_id].pos_z;
	}
	if (player[g_id].rotate_y == 1.5f * XM_PI)
	{
		weaponX = player[g_id].pos_x;
		weaponY = player[g_id].pos_y + WEAPON_UPFROMFLOOR;
		weaponZ = player[g_id].pos_z - WEAPON_POSFROMPLAYER;
	}

	// 대각선 4방향 (무기)

	if (player[g_id].rotate_y == 0.25f * XM_PI)
	{
		weaponX = player[g_id].pos_x - cos(XM_PI / 4) * WEAPON_POSFROMPLAYER;
		weaponY = player[g_id].pos_y + WEAPON_UPFROMFLOOR;
		weaponZ = player[g_id].pos_z + sin(XM_PI / 4) * WEAPON_POSFROMPLAYER;
	}
	if (player[g_id].rotate_y == 0.75f * XM_PI)
	{
		weaponX = player[g_id].pos_x + cos(XM_PI / 4) * WEAPON_POSFROMPLAYER;
		weaponY = player[g_id].pos_y + WEAPON_UPFROMFLOOR;
		weaponZ = player[g_id].pos_z + sin(XM_PI / 4) * WEAPON_POSFROMPLAYER;
	}
	if (player[g_id].rotate_y == 1.25f * XM_PI)
	{
		weaponX = player[g_id].pos_x + cos(XM_PI / 4) * WEAPON_POSFROMPLAYER;
		weaponY = player[g_id].pos_y + WEAPON_UPFROMFLOOR;
		weaponZ = player[g_id].pos_z - sin(XM_PI / 4) * WEAPON_POSFROMPLAYER;
	}
	if (player[g_id].rotate_y == 1.75f * XM_PI)
	{
		weaponX = player[g_id].pos_x - cos(XM_PI / 4) * WEAPON_POSFROMPLAYER;
		weaponY = player[g_id].pos_y + WEAPON_UPFROMFLOOR;
		weaponZ = player[g_id].pos_z - sin(XM_PI / 4) * WEAPON_POSFROMPLAYER;
	}

	auto weaponRitem = std::make_unique<RenderItem>();
	XMStoreFloat4x4
	(
		// 플레이어 배율, 회전, 위치에 맞춰 지정
		&weaponRitem->World,
		XMMatrixScaling(1.0f, 1.0f, 1.0f)
		* XMMatrixRotationRollPitchYaw(player[g_id].rotate_x, player[g_id].rotate_y, player[g_id].rotate_z)
		* XMMatrixTranslation(weaponX, weaponY, weaponZ)
	);
	weaponRitem->TexTransform = MathHelper::Identity4x4();
	weaponRitem->ObjCBIndex = INDEXWEAPON;
	weaponRitem->Mat = mMaterials["weaponMat"].get();
	weaponRitem->Geo = mGeometries["shapeGeo"].get();
	weaponRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	weaponRitem->IndexCount = weaponRitem->Geo->DrawArgs["weapon"].IndexCount;
	weaponRitem->StartIndexLocation = weaponRitem->Geo->DrawArgs["weapon"].StartIndexLocation;
	weaponRitem->BaseVertexLocation = weaponRitem->Geo->DrawArgs["weapon"].BaseVertexLocation;
	mAllRitems.push_back(std::move(weaponRitem));

	// 방패

	float shieldX = 0.0f;
	float shieldY = 0.0f;
	float shieldZ = 0.0f;

	// 직선 4방향 (무기)

	if (player[g_id].rotate_y == 0.0f * XM_PI)
	{
		shieldX = player[g_id].pos_x + SHIELD_POSFROMPLAYER;
		shieldY = player[g_id].pos_y + SHIELD_UPFROMFLOOR;
		shieldZ = player[g_id].pos_z;
	}
	if (player[g_id].rotate_y == 0.5f * XM_PI)
	{
		shieldX = player[g_id].pos_x;
		shieldY = player[g_id].pos_y + SHIELD_UPFROMFLOOR;
		shieldZ = player[g_id].pos_z - SHIELD_POSFROMPLAYER;
	}
	if (player[g_id].rotate_y == 1.0f * XM_PI)
	{
		shieldX = player[g_id].pos_x - SHIELD_POSFROMPLAYER;
		shieldY = player[g_id].pos_y + SHIELD_UPFROMFLOOR;
		shieldZ = player[g_id].pos_z;
	}
	if (player[g_id].rotate_y == 1.5f * XM_PI)
	{
		shieldX = player[g_id].pos_x;
		shieldY = player[g_id].pos_y + SHIELD_UPFROMFLOOR;
		shieldZ = player[g_id].pos_z + SHIELD_POSFROMPLAYER;
	}

	// 대각선 4방향 (무기)

	if (player[g_id].rotate_y == 0.25f * XM_PI)
	{
		shieldX = player[g_id].pos_x + cos(XM_PI / 4) * SHIELD_POSFROMPLAYER;
		shieldY = player[g_id].pos_y + SHIELD_UPFROMFLOOR;
		shieldZ = player[g_id].pos_z - sin(XM_PI / 4) * SHIELD_POSFROMPLAYER;
	}
	if (player[g_id].rotate_y == 0.75f * XM_PI)
	{
		shieldX = player[g_id].pos_x - cos(XM_PI / 4) * SHIELD_POSFROMPLAYER;
		shieldY = player[g_id].pos_y + SHIELD_UPFROMFLOOR;
		shieldZ = player[g_id].pos_z - sin(XM_PI / 4) * SHIELD_POSFROMPLAYER;
	}
	if (player[g_id].rotate_y == 1.25f * XM_PI)
	{
		shieldX = player[g_id].pos_x - cos(XM_PI / 4) * SHIELD_POSFROMPLAYER;
		shieldY = player[g_id].pos_y + SHIELD_UPFROMFLOOR;
		shieldZ = player[g_id].pos_z + sin(XM_PI / 4) * SHIELD_POSFROMPLAYER;
	}
	if (player[g_id].rotate_y == 1.75f * XM_PI)
	{
		shieldX = player[g_id].pos_x + cos(XM_PI / 4) * SHIELD_POSFROMPLAYER;
		shieldY = player[g_id].pos_y + SHIELD_UPFROMFLOOR;
		shieldZ = player[g_id].pos_z + sin(XM_PI / 4) * SHIELD_POSFROMPLAYER;
	}

	auto shieldRitem = std::make_unique<RenderItem>();
	XMStoreFloat4x4
	(
		// 플레이어 배율, 회전, 위치에 맞춰 지정
		&shieldRitem->World,
		XMMatrixScaling(1.0f, 1.0f, 1.0f)
		* XMMatrixRotationRollPitchYaw(player[g_id].rotate_x, player[g_id].rotate_y, player[g_id].rotate_z)
		* XMMatrixTranslation(shieldX, shieldY, shieldZ)
	);
	shieldRitem->TexTransform = MathHelper::Identity4x4();
	shieldRitem->ObjCBIndex = INDEXSHIELD;
	shieldRitem->Mat = mMaterials["shieldMat"].get();
	shieldRitem->Geo = mGeometries["shapeGeo"].get();
	shieldRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	shieldRitem->IndexCount = shieldRitem->Geo->DrawArgs["shield"].IndexCount;
	shieldRitem->StartIndexLocation = shieldRitem->Geo->DrawArgs["shield"].StartIndexLocation;
	shieldRitem->BaseVertexLocation = shieldRitem->Geo->DrawArgs["shield"].BaseVertexLocation;
	mAllRitems.push_back(std::move(shieldRitem));
}

void SoulSimul::Draw(const GameTimer& gt)
{
    auto cmdListAlloc = mCurrFrameResource->CmdListAlloc;

    // Reuse the memory associated with command recording.
    // We can only reset when the associated command lists have finished execution on the GPU.
    ThrowIfFailed(cmdListAlloc->Reset());

    // A command list can be reset after it has been added to the command queue via ExecuteCommandList.
    // Reusing the command list reuses memory.
    ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mOpaquePSO.Get()));

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

	auto passCB = mCurrFrameResource->PassCB->Resource();
	mCommandList->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());

    DrawRenderItems(mCommandList.Get(), mOpaqueRitems);

    // Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    // Done recording commands.
    ThrowIfFailed(mCommandList->Close());

    // Add the command list to the queue for execution.
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Swap the back and front buffers
    ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

    // Advance the fence value to mark commands up to this fence point.
    mCurrFrameResource->Fence = ++mCurrentFence;

    // Add an instruction to the command queue to set a new fence point. 
    // Because we are on the GPU timeline, the new fence point won't be 
    // set until the GPU finishes processing all the commands prior to this Signal().
    mCommandQueue->Signal(mFence.Get(), mCurrentFence);
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
	// 왼쪽 마우스 버튼
	if ((btnState & MK_LBUTTON) != 0)
	{
		/*
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta -= dx;
		mPhi += dy;

		// 아래에서는 안 보이게 각도 제한
		mPhi = MathHelper::Clamp(mPhi, 0.5f, MathHelper::Pi - 2.0f);
		*/
	}

	// 오른쪽 마우스 버튼
	else if ((btnState & MK_RBUTTON) != 0)
	{
		/*
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f * static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f * static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, 5.0f, 15.0f);
		*/
	}

    mLastMousePos.x = x;
    mLastMousePos.y = y;
}
 
void SoulSimul::UpdateCamera(const GameTimer& gt)
{
    // 플레이어 위치
    float fPlayerPosX = player[g_id].pos_x;
    float fPlayerPosY = player[g_id].pos_y;
    float fPlayerPosZ = player[g_id].pos_z;

    // 플레이어로부터 카메라 위치
    float fBehindPlayer = 10.0f; // 플레이어로부터 뒤 거리
    float fAbovePlayer = player[g_id].fRadius; // 플레이어로부터 위 거리 

	// 카메라 위치 좌표값
    float cameraPosX = fPlayerPosX + fBehindPlayer * cosf(mTheta);
    float cameraPosY = fPlayerPosY + fAbovePlayer;
	float cameraPosZ = fPlayerPosZ + fBehindPlayer * sinf(mTheta);

    // 카메라 위치 지정
    XMVECTOR pos = XMVectorSet(cameraPosX, cameraPosY, cameraPosZ, 1.0f);
    
    // target (플레이어 좌표를 중심으로)
    XMVECTOR target = XMVectorSet(fPlayerPosX, fPlayerPosY, fPlayerPosZ, 1.0f);

    // up 벡터
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    // 카메라 뷰 적용
    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);
}

void SoulSimul::AnimateMaterials(const GameTimer& gt)
{
	
}

void SoulSimul::UpdateObjectCBs(const GameTimer& gt)
{
	auto currObjectCB = mCurrFrameResource->ObjectCB.get();
	for(auto& e : mAllRitems)
	{
		// Only update the cbuffer data if the constants have changed.  
		// This needs to be tracked per frame resource.
		if(e->NumFramesDirty > 0)
		{
			XMMATRIX world = XMLoadFloat4x4(&e->World);
			XMMATRIX texTransform = XMLoadFloat4x4(&e->TexTransform);

			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(texTransform));

			currObjectCB->CopyData(e->ObjCBIndex, objConstants);

			// Next FrameResource need to be updated too.
			e->NumFramesDirty--;
		}
	}
}

void SoulSimul::UpdateMaterialCBs(const GameTimer& gt)
{
	auto currMaterialCB = mCurrFrameResource->MaterialCB.get();
	for(auto& e : mMaterials)
	{
		// Only update the cbuffer data if the constants have changed.  If the cbuffer
		// data changes, it needs to be updated for each FrameResource.
		Material* mat = e.second.get();
		if(mat->NumFramesDirty > 0)
		{
			XMMATRIX matTransform = XMLoadFloat4x4(&mat->MatTransform);

			MaterialConstants matConstants;
			matConstants.DiffuseAlbedo = mat->DiffuseAlbedo;
			matConstants.FresnelR0 = mat->FresnelR0;
			matConstants.Roughness = mat->Roughness;
			XMStoreFloat4x4(&matConstants.MatTransform, XMMatrixTranspose(matTransform));

			currMaterialCB->CopyData(mat->MatCBIndex, matConstants);

			// Next FrameResource need to be updated too.
			mat->NumFramesDirty--;
		}
	}
}

void SoulSimul::UpdateMainPassCB(const GameTimer& gt)
{
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);

	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

	XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
	mMainPassCB.EyePosW = mEyePos;
	mMainPassCB.RenderTargetSize = XMFLOAT2((float)mClientWidth, (float)mClientHeight);
	mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / mClientWidth, 1.0f / mClientHeight);
	mMainPassCB.NearZ = 1.0f;
	mMainPassCB.FarZ = 1000.0f;
	mMainPassCB.TotalTime = gt.TotalTime();
	mMainPassCB.DeltaTime = gt.DeltaTime();
	mMainPassCB.AmbientLight = { 0.25f, 0.25f, 0.35f, 1.0f };
	mMainPassCB.Lights[0].Direction = { 0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[0].Strength = { 0.6f, 0.6f, 0.6f };
	mMainPassCB.Lights[1].Direction = { -0.57735f, -0.57735f, 0.57735f };
	mMainPassCB.Lights[1].Strength = { 0.3f, 0.3f, 0.3f };
	mMainPassCB.Lights[2].Direction = { 0.0f, -0.707f, -0.707f };
	mMainPassCB.Lights[2].Strength = { 0.15f, 0.15f, 0.15f };

	auto currPassCB = mCurrFrameResource->PassCB.get();
	currPassCB->CopyData(0, mMainPassCB);
}

void SoulSimul::BuildRootSignature()
{
	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[3];

	// Create root CBV.
	slotRootParameter[0].InitAsConstantBufferView(0);
	slotRootParameter[1].InitAsConstantBufferView(1);
	slotRootParameter[2].InitAsConstantBufferView(2);

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(3, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if(errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature.GetAddressOf())));
}

void SoulSimul::BuildShadersAndInputLayout()
{
	const D3D_SHADER_MACRO alphaTestDefines[] =
	{
		"ALPHA_TEST", "1",
		NULL, NULL
	};

	mShaders["standardVS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["opaquePS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", nullptr, "PS", "ps_5_1");
	
    mInputLayout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };
}

void SoulSimul::BuildShapeGeometry()
{
    GeometryGenerator geoGen;
	GeometryGenerator::MeshData floor = geoGen.CreateGrid(20.0f, 20.0f, 40, 40);
	GeometryGenerator::MeshData weapon = geoGen.CreateBox(0.5f, 3.0f, 0.5f, 3);
	GeometryGenerator::MeshData shield = geoGen.CreateBox(1.4f, 1.4f, 0.2f, 3);
	// GeometryGenerator::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20);
	// GeometryGenerator::MeshData cylinder = geoGen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20);

	//
	// We are concatenating all the geometry into one big vertex/index buffer.  So
	// define the regions in the buffer each submesh covers.
	//

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	UINT floorVertexOffset = 0;
	UINT weaponVertexOffset = floorVertexOffset + (UINT)floor.Vertices.size();
	UINT shieldVertexOffset = weaponVertexOffset + (UINT)weapon.Vertices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	UINT floorIndexOffset = 0;
	UINT weaponIndexOffset = floorIndexOffset + (UINT)floor.Indices32.size();
	UINT shieldIndexOffset = weaponIndexOffset + (UINT)weapon.Indices32.size();

	// 각 메쉬들의 위치를 지정
	SubmeshGeometry floorSubmesh;
	floorSubmesh.IndexCount = (UINT)floor.Indices32.size();
	floorSubmesh.StartIndexLocation = floorIndexOffset;
	floorSubmesh.BaseVertexLocation = floorVertexOffset;

	SubmeshGeometry weaponSubmesh;
	weaponSubmesh.IndexCount = (UINT)weapon.Indices32.size();
	weaponSubmesh.StartIndexLocation = weaponIndexOffset;
	weaponSubmesh.BaseVertexLocation = weaponVertexOffset;

	SubmeshGeometry shieldSubmesh;
	shieldSubmesh.IndexCount = (UINT)shield.Indices32.size();
	shieldSubmesh.StartIndexLocation = shieldIndexOffset;
	shieldSubmesh.BaseVertexLocation = shieldVertexOffset;

	// 총합 버텍스 수
	auto totalVertexCount =
		floor.Vertices.size() +
		weapon.Vertices.size() +
		shield.Vertices.size();

	std::vector<Vertex> vertices(totalVertexCount);
	UINT k = 0;

	for(size_t i = 0; i < floor.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = floor.Vertices[i].Position;
		vertices[k].Normal = floor.Vertices[i].Normal;
	}

	for(size_t i = 0; i < weapon.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = weapon.Vertices[i].Position;
		vertices[k].Normal = weapon.Vertices[i].Normal;
	}

	for(size_t i = 0; i < shield.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = shield.Vertices[i].Position;
		vertices[k].Normal = shield.Vertices[i].Normal;
	}

	std::vector<std::uint16_t> indices;
	indices.insert(indices.end(), std::begin(floor.GetIndices16()), std::end(floor.GetIndices16()));
	indices.insert(indices.end(), std::begin(weapon.GetIndices16()), std::end(weapon.GetIndices16()));
	indices.insert(indices.end(), std::begin(shield.GetIndices16()), std::end(shield.GetIndices16()));

    const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
    const UINT ibByteSize = (UINT)indices.size()  * sizeof(std::uint16_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "shapeGeo";

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
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	geo->DrawArgs["floor"] = floorSubmesh;
	geo->DrawArgs["weapon"] = weaponSubmesh;
	geo->DrawArgs["shield"] = shieldSubmesh;

	mGeometries[geo->Name] = std::move(geo);
}

void SoulSimul::BuildSkullGeometry()
{
	std::ifstream fin("Models/skull.txt");

	if(!fin)
	{
		MessageBox(0, L"Models/skull.txt not found.", 0, 0);
		return;
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	std::vector<Vertex> vertices(vcount);
	for(UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;
	}

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	std::vector<std::int32_t> indices(3 * tcount);
	for(UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();

	//
	// Pack the indices of all the meshes into one index buffer.
	//

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

void SoulSimul::BuildPSOs()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc;

	//
	// PSO for opaque objects.
	//
    ZeroMemory(&opaquePsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	opaquePsoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	opaquePsoDesc.pRootSignature = mRootSignature.Get();
	opaquePsoDesc.VS = 
	{ 
		reinterpret_cast<BYTE*>(mShaders["standardVS"]->GetBufferPointer()), 
		mShaders["standardVS"]->GetBufferSize()
	};
	opaquePsoDesc.PS = 
	{ 
		reinterpret_cast<BYTE*>(mShaders["opaquePS"]->GetBufferPointer()),
		mShaders["opaquePS"]->GetBufferSize()
	};
	opaquePsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	opaquePsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	opaquePsoDesc.SampleMask = UINT_MAX;
	opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	opaquePsoDesc.NumRenderTargets = 1;
	opaquePsoDesc.RTVFormats[0] = mBackBufferFormat;
	opaquePsoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	opaquePsoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	opaquePsoDesc.DSVFormat = mDepthStencilFormat;
    ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&opaquePsoDesc, IID_PPV_ARGS(&mOpaquePSO)));
}

void SoulSimul::BuildFrameResources()
{
    for(int i = 0; i < gNumFrameResources; ++i)
    {
        mFrameResources.push_back(std::make_unique<FrameResource>(md3dDevice.Get(),
            1, (UINT)mAllRitems.size(), (UINT)mMaterials.size()));
    }
}

// 재질 build (코드 간략화)
void SoulSimul::BuildMaterial(char* name, int index, 
	XMFLOAT4 DiffuseAlbedo, XMFLOAT3 FresnelR0, float roughness)
{
	auto material = std::make_unique<Material>();
	material->Name = name;
	material->MatCBIndex = index;
	material->DiffuseSrvHeapIndex = index;
	material->DiffuseAlbedo = DiffuseAlbedo;
	material->FresnelR0 = FresnelR0;
	material->Roughness = roughness;
	mMaterials[name] = std::move(material);
}

void SoulSimul::BuildMaterials()
{
	// 바닥
	BuildMaterial
	(
		"floorMat", INDEXFLOOR,             // material 이름 및 인덱스
		XMFLOAT4(0.95f, 0.80f, 0.0f, 1.0f), // DiffuseAlbedo
		XMFLOAT3(0.02f, 0.02f, 0.02f), 0.2f // FresnelR0, Roughness
	);

	// 플레이어
	BuildMaterial
	(
		"skullMat", INDEXPLAYER,            // material 이름 및 인덱스
		XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f),   // DiffuseAlbedo
		XMFLOAT3(0.05f, 0.05f, 0.05f), 0.3f // FresnelR0, Roughness
	);

	// 무기
	BuildMaterial
	(
		"weaponMat", INDEXWEAPON,           // material 이름 및 인덱스
		XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f),   // DiffuseAlbedo
		XMFLOAT3(0.02f, 0.02f, 0.02f), 0.1f // FresnelR0, Roughness
	);

	// 방패
	BuildMaterial
	(
		"shieldMat", INDEXSHIELD,           // material 이름 및 인덱스
		XMFLOAT4(0.65f, 0.65f, 0.8f, 1.0f), // DiffuseAlbedo
		XMFLOAT3(0.05f, 0.05f, 0.05f), 0.1f // FresnelR0, Roughness
	);
}

void SoulSimul::BuildRenderItems()
{
	// 바닥 격자 그리기
    auto floorRitem = std::make_unique<RenderItem>();
	floorRitem->World = MathHelper::Identity4x4();
	XMStoreFloat4x4(&floorRitem->TexTransform, XMMatrixScaling(8.0f, 8.0f, 1.0f));
	floorRitem->ObjCBIndex = INDEXFLOOR;
	floorRitem->Mat = mMaterials["floorMat"].get();
	floorRitem->Geo = mGeometries["shapeGeo"].get();
	floorRitem->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	floorRitem->IndexCount = floorRitem->Geo->DrawArgs["floor"].IndexCount;
	floorRitem->StartIndexLocation = floorRitem->Geo->DrawArgs["floor"].StartIndexLocation;
	floorRitem->BaseVertexLocation = floorRitem->Geo->DrawArgs["floor"].BaseVertexLocation;
	mAllRitems.push_back(std::move(floorRitem));

	// 플레이어 업데이트
	UpdatePlayer();

	// All the render items are opaque.
	for(auto& e : mAllRitems)
		mOpaqueRitems.push_back(e.get());
}

void SoulSimul::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems)
{
    UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
    UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));
 
	auto objectCB = mCurrFrameResource->ObjectCB->Resource();
	auto matCB = mCurrFrameResource->MaterialCB->Resource();

    // For each render item...
    for(size_t i = 0; i < ritems.size(); ++i)
    {
        auto ri = ritems[i];

        cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());
        cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
        cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

        D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + ri->ObjCBIndex*objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + ri->Mat->MatCBIndex*matCBByteSize;

        cmdList->SetGraphicsRootConstantBufferView(0, objCBAddress);
		cmdList->SetGraphicsRootConstantBufferView(1, matCBAddress);

        cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
    }
}
