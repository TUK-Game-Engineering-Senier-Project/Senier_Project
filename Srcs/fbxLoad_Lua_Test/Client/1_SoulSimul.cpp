//***************************************************************************************
// SoulSimul.cpp by Frank Luna (C) 2015 All Rights Reserved.
//***************************************************************************************

// ���� Client���Ͽ��� ���� ���� �����ϰ� �����¿� �̵��� ���ܵ� ����
// fbx sdk �ٿ�ε�� �ִ� sample ������ �ִ� humanoid.fbx ���
// humanoid.fbx�� Client ���� ���� ����, ��ġ ����� ����� ��ġ �����ϱ�
// ���� �ִ�, ���г� ���⸦ ����ϴ� �ڵ�� ���� �ǹ� ����

// Visual Studio���� (���� ���� ����)
// fbx sdk ���� #include <fbxsdk.h>�� �ʼ��� �� ��
// - C / C++ - �Ϲ� - �߰� ���� ���͸� ���� : include ��� D:\FBX_SDK\2020.3.7\include ����
// - �Ӽ� - ��Ŀ - �Ϲ� - �߰� ���̺귯�� ���͸� ���� : FBX SDK ��� D:\FBX_SDK\2020.3.7\lib\x64\debug ����

// �߰��� �͵�
// void SoulSimul::BuildFbxGeometry(const char* filename, const char* subMeshName, const char* geoName, 
//     float scaleMulX, float scaleMulY, float scaleMulZ);
// void SoulSimul::BuildObject(const char* name, const char* mat, const char* geo, Object c)

// Player Ŭ������ Character Ŭ������ ����
// ������ �ٴڵ� fbx�� �ҷ��� ���̱� ������ 
// - UpdateObject�� BuildItem�� BuildObject�� ������
// ���� �ٴ��̶� �Ǿ��ִ� �� �߾��� ǥ���ϴ� �� ���� ������ü

#include "../Common/d3dApp.h"
#include "../Common/MathHelper.h"
#include "../Common/UploadBuffer.h"
#include "../Common/GeometryGenerator.h"
#include "FrameResource.h"

#include <fbxsdk.h>
#include "LuaObject.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

// ������ ���ҽ� ����
const int gNumFrameResources = 3;

// �� ������Ʈ �ε���
constexpr char INDEXPLAYER = 0;
constexpr char INDEXFLOOR = 1;
constexpr char INDEXENEMY = 2;

// lua ���
lua_State* L = luaL_newstate();

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

// RenderItem ���� ����
std::vector<std::unique_ptr<RenderItem>> moveAbleItems;

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

	// ������Ʈ ����
	void BuildObject(const char* name, const char* mat, const char* geo, Object c, const int code);

	// ī�޶� ������Ʈ (�÷��̾� ��ġ Ȯ���� ��)
	void UpdateCamera(const GameTimer& gt);

	void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMaterialCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);

    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildShapeGeometry();

	// FBX ���� (���ϸ�, geo �̸�, subMesh �̸�, �ҷ��� �� ũ�� ����)
	void SoulSimul::BuildFbxGeometry(const char* filename, const char* subMeshName, const char* geoName,
		float scaleMulX, float scaleMulY, float scaleMulZ);

    void BuildPSOs();
    void BuildFrameResources();

	// ���� build (�ڵ� ����ȭ)
	void BuildMaterial(char* name, int index,
		XMFLOAT4 DiffuseAlbedo, XMFLOAT3 FresnelR0, float roughness);

	// ���� ��ü ����
	void BuildMaterials();

	// ������ ���� (�ڵ� ����ȭ)
	void BuildItem(const char* name, const char* mat, const char* geo);

	// ������ ��ü ���� �� �׸���
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

    float mTheta  = 1.5f * XM_PI;
    float mPhi    = 0.2f * XM_PI;
    float mRadius = 10.0f;

	POINT mLastMousePos = { 0, 0 };
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
	// ----- Lua ���� -----
	luaL_openlibs(L);
	luaL_loadfile(L, "LuaObject.lua");
	lua_pcall(L, 0, 0, 0);

	// ### �׽�Ʈ [���]�� �Լ� lua�� ���
	// lua_registerLuaDebugOutput(L);
}

SoulSimul::~SoulSimul()
{
    if(md3dDevice != nullptr)
        FlushCommandQueue();

	// ----- lua �ݱ� -----
	lua_close(L);
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

	// �÷��̾� ������Ʈ �߰� (ü��, ��ǥ)
	lua_newObject(L, "player1", 100, -4.0f, 0.0f, -3.0f);

	// �� ������Ʈ �߰� (ü��, ��ǥ)
	lua_newObject(L, "enemy_m", 100, 5.0f, 0.0f, 2.0f);

	// �÷��̾� ��� ���� 
	// �Ű����� : ���ϸ�, subMesh �̸�, geo �̸�, �ҷ��� �� ũ�� ����
	BuildFbxGeometry("sample_humanoid.fbx", "player", "playerGeo", 0.017f, 0.017f, 0.017f);

	// ### ���� �� ���� (�ӽ�)
	BuildFbxGeometry("sample_humanoid.fbx", "enemy_m", "enemy_mGeo", 0.025f, 0.025f, 0.025f);

	// ### �ٴ� ���� (�ӽ�)
	BuildFbxGeometry("sample_box.fbx", "floor", "floor_mGeo", 0.04f, 0.02f, 0.04f);

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
	lua_BehaviorTree(L, "enemy_m", "player1", player[0].pos_x, player[0].pos_z); // �ൿ Ʈ�� ����
	lua_DoAction(L, "enemy_m", "player1", player[0].pos_x, player[0].pos_z);     // �ൿ Ʈ���� ���� ���� ����

	// --- ������ ������Ʈ ---

	// �� ��ġ ������Ʈ
	enemy_m.pos_x = lua_getFuncFloat(L, "enemy_m", "GetData", "pos_x");
	enemy_m.pos_y = lua_getFuncFloat(L, "enemy_m", "GetData", "pos_y");
	enemy_m.pos_z = lua_getFuncFloat(L, "enemy_m", "GetData", "pos_z");

	// �� ȸ�� ������Ʈ
	enemy_m.rotate_x = lua_getFuncFloat(L, "enemy_m", "GetData", "rotate_x");
	enemy_m.rotate_y = lua_getFuncFloat(L, "enemy_m", "GetData", "rotate_y");
	enemy_m.rotate_z = lua_getFuncFloat(L, "enemy_m", "GetData", "rotate_z");
	
	// �÷��̾� ��ġ ������Ʈ

	if (lua_getFuncFloat(L, "player1", "GetData", "hit_now") == 0)
	{
		lua_updateObject(L, "player1", 0, player[0].pos_x, player[0].pos_y, player[0].pos_z,
			player[0].rotate_x, player[0].rotate_y, player[0].rotate_z);
	}
	else if (lua_getFuncFloat(L, "player1", "GetData", "hit_now") == 1)
	{
		player[0].pos_x = lua_getFuncFloat(L, "player1", "GetData", "pos_x");
		player[0].pos_y = lua_getFuncFloat(L, "player1", "GetData", "pos_y");
		player[0].pos_z = lua_getFuncFloat(L, "player1", "GetData", "pos_z");
	}

	// --- �����Ͱ� ������Ʈ�� �Ŀ� ������Ʈ�� ������ ---

	// �÷��̾� ���� 
	BuildObject("player", "playerMat", "playerGeo", player[0], INDEXPLAYER);
	// ### ���⿡ �ٸ� �÷��̾ �߰��� ��

	// ### ���� �� ���� (�ӽ�)
	BuildObject("enemy_m", "enemy_mMat", "enemy_mGeo", enemy_m, INDEXENEMY);
	// ### ���⿡ �ٸ� ���� �߰��� ��

	// ī�޶� ������Ʈ (������Ʈ�� ��ġ Ȯ���� ��)
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

	UpdateObjectCBs(gt);
	UpdateMaterialCBs(gt);
	UpdateMainPassCB(gt);
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
	// ���� ���콺 ��ư
	if ((btnState & MK_LBUTTON) != 0)
	{
		/*
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta -= dx;
		mPhi += dy;

		// �Ʒ������� �� ���̰� ���� ����
		mPhi = MathHelper::Clamp(mPhi, 0.5f, MathHelper::Pi - 2.0f);
		*/
	}

	// ������ ���콺 ��ư
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
    // �÷��̾� ��ġ
	float fPlayerPosX = player[g_id].pos_x;
	float fPlayerPosY = player[g_id].pos_y;
	float fPlayerPosZ = player[g_id].pos_z;

    // �÷��̾�κ��� ī�޶� ��ġ
    float fBehindPlayer = 12.0f; // �÷��̾�κ��� �� �Ÿ�
    float fAbovePlayer  = 10.0f; // �÷��̾�κ��� �� �Ÿ� 

	// ī�޶� ��ġ ��ǥ��
    float cameraPosX = fPlayerPosX + fBehindPlayer * cosf(mTheta);
    float cameraPosY = fPlayerPosY + fAbovePlayer;
	float cameraPosZ = fPlayerPosZ + fBehindPlayer * sinf(mTheta);

    // ī�޶� ��ġ ����
    XMVECTOR pos = XMVectorSet(cameraPosX, cameraPosY, cameraPosZ, 1.0f);
    
    // target (�÷��̾� ��ǥ�� �߽�����)
    XMVECTOR target = XMVectorSet(fPlayerPosX, fPlayerPosY, fPlayerPosZ, 1.0f);

    // up ����
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    // ī�޶� �� ����
    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);
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

	// ����
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

	// We are concatenating all the geometry into one big vertex/index buffer.  So
	// define the regions in the buffer each submesh covers.

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	UINT floorVertexOffset = 0;
	UINT weaponVertexOffset = floorVertexOffset + (UINT)floor.Vertices.size();
	UINT shieldVertexOffset = weaponVertexOffset + (UINT)weapon.Vertices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	UINT floorIndexOffset = 0;
	UINT weaponIndexOffset = floorIndexOffset + (UINT)floor.Indices32.size();
	UINT shieldIndexOffset = weaponIndexOffset + (UINT)weapon.Indices32.size();

	// �� �޽����� ��ġ�� ����

	SubmeshGeometry floorSubmesh; // �ٴ�
	floorSubmesh.IndexCount = (UINT)floor.Indices32.size();
	floorSubmesh.StartIndexLocation = floorIndexOffset;
	floorSubmesh.BaseVertexLocation = floorVertexOffset;
	SubmeshGeometry weaponSubmesh; // ����
	weaponSubmesh.IndexCount = (UINT)weapon.Indices32.size();
	weaponSubmesh.StartIndexLocation = weaponIndexOffset;
	weaponSubmesh.BaseVertexLocation = weaponVertexOffset;
	SubmeshGeometry shieldSubmesh; // ����
	shieldSubmesh.IndexCount = (UINT)shield.Indices32.size();
	shieldSubmesh.StartIndexLocation = shieldIndexOffset;
	shieldSubmesh.BaseVertexLocation = shieldVertexOffset;

	// ���� ���ؽ� ��
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

void SoulSimul::BuildFbxGeometry(const char* filename, const char* subMeshName, const char* geoName,
	float scaleMulX, float scaleMulY, float scaleMulZ)
{
	// FBX SDK �ʱ�ȭ
	FbxManager* sdkManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(sdkManager, IOSROOT);
	sdkManager->SetIOSettings(ios);

	// importer ����
	FbxImporter* importer = FbxImporter::Create(sdkManager, "");

	// importer �ʱ�ȭ ������ ����
	if (!importer->Initialize(filename, -1, sdkManager->GetIOSettings()))
	{
		importer->Destroy();
		sdkManager->Destroy();
		return;
	}

	// �ҷ��� �����͸� ������ scene �����
	FbxScene* scene = FbxScene::Create(sdkManager, "Scene");

	// scene import�ϱ�
	importer->Import(scene);
	importer->Destroy();

	FbxNode* rootNode = scene->GetRootNode();
	FbxNode* meshNode = nullptr;

	if (rootNode)
	{
		for (int i = 0; i < rootNode->GetChildCount(); ++i)
		{
			meshNode = rootNode->GetChild(i);
			if (meshNode->GetNodeAttribute() && meshNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
				break;
			meshNode = nullptr;
		}
	}

	if (!meshNode)
	{
		scene->Destroy();
		sdkManager->Destroy();
		return;
	}

	// mesh���� vertex �� index ������ ����
	FbxMesh* mesh = static_cast<FbxMesh*>(meshNode->GetNodeAttribute());
	if (!mesh)
	{
		scene->Destroy();
		sdkManager->Destroy();
		return;
	}

	int numVertices = mesh->GetControlPointsCount();
	FbxVector4* vertices = mesh->GetControlPoints();
	int numIndices = mesh->GetPolygonVertexCount();
	int* indices = mesh->GetPolygonVertices();

	std::vector<Vertex> vertexData;
	std::vector<std::int32_t> indexData;

	for (int i = 0; i < numVertices; ++i)
	{
		Vertex vertex;
		// ũ�� ���� ����
		vertex.Pos = DirectX::XMFLOAT3(static_cast<float>(vertices[i][0]) * scaleMulX,
			static_cast<float>(vertices[i][1]) * scaleMulY,
			static_cast<float>(vertices[i][2]) * scaleMulZ);
		vertex.Normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertexData.push_back(vertex);
	}

	for (int i = 0; i < numIndices; ++i)
	{
		indexData.push_back(indices[i]);
	}

	// vertex ���� �� index ���� �����ϰ� ��� ó��
	const UINT vbByteSize = static_cast<UINT>(vertexData.size()) * sizeof(Vertex);
	const UINT ibByteSize = static_cast<UINT>(indexData.size()) * sizeof(std::int32_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = geoName;

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertexData.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indexData.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertexData.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indexData.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R32_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	// �÷��̾� SubMesh ���� �� ����
	SubmeshGeometry subMesh;
	subMesh.IndexCount = static_cast<UINT>(indexData.size());
	subMesh.StartIndexLocation = 0;
	subMesh.BaseVertexLocation = 0;
	geo->DrawArgs[subMeshName] = subMesh;

	mGeometries[geo->Name] = std::move(geo);

	// scene�� sdkManager ������ ����
	scene->Destroy();
	sdkManager->Destroy();
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

// ���� build (�ڵ� ����ȭ)
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

// ���� ��ü build
void SoulSimul::BuildMaterials()
{
	// �ٴ� ����
	BuildMaterial
	(
		"floorMat", INDEXFLOOR,             // material �̸� �� �ε���
		XMFLOAT4(0.95f, 0.80f, 0.0f, 1.0f), // DiffuseAlbedo
		XMFLOAT3(0.02f, 0.02f, 0.02f), 0.2f // FresnelR0, Roughness
	);

	// �÷��̾� ����
	BuildMaterial
	(
		"playerMat", INDEXPLAYER,           // material �̸� �� �ε���
		XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f),   // DiffuseAlbedo
		XMFLOAT3(0.05f, 0.05f, 0.05f), 0.3f // FresnelR0, Roughness
	);

	// ### ���� �� ���� (�ӽ�)
	BuildMaterial
	(
		"enemy_mMat", INDEXENEMY,           // material �̸� �� �ε���
		XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f),   // DiffuseAlbedo
		XMFLOAT3(0.05f, 0.05f, 0.05f), 0.4f // FresnelR0, Roughness
	);

	// ### �ʿ�� ���� �߰��ϱ� (BuildMaterial())
}

// ������Ʈ ����
// Character Ŭ���� ���� �Է¹޴� ������ 
// - �ش� Ŭ�������� ����, ȸ��, ��ġ ���� �޾� ����ϱ� �����̴�.

void SoulSimul::BuildObject(const char* name, const char* mat, const char* geo, Object obj, const int code)
{
	// objcode�� BuildObject �� ��ü�� �����ϱ� ���� int�� �ڵ�

	// ũ�� ����
	if (code >= moveAbleItems.size()) {
		moveAbleItems.resize(code + 1);
	}

	// RenderItem ������ ����
	moveAbleItems[code] = std::make_unique<RenderItem>();

	// ������Ʈ ����
	XMStoreFloat4x4
	(
		// �ҷ��� Character c�� �ش��ϴ� ����, ȸ��, ��ġ�� ���� ����
		&moveAbleItems[code]->World,
		XMMatrixScaling(obj.scale_x, obj.scale_y, obj.scale_z)
		* XMMatrixRotationRollPitchYaw(obj.rotate_x, obj.rotate_y, obj.rotate_z)
		* XMMatrixTranslation(obj.pos_x, obj.pos_y, obj.pos_z)
	);

	// BuildItem �ڵ�� ���� ����

	moveAbleItems[code]->TexTransform = MathHelper::Identity4x4();
	moveAbleItems[code]->ObjCBIndex = code; // ���⼭ code�� �� ������Ʈ�� ���� �ٸ�
	moveAbleItems[code]->Mat = mMaterials[mat].get();
	moveAbleItems[code]->Geo = mGeometries[geo].get();
	moveAbleItems[code]->PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	moveAbleItems[code]->IndexCount = moveAbleItems[code]->Geo->DrawArgs[name].IndexCount;
	moveAbleItems[code]->StartIndexLocation = moveAbleItems[code]->Geo->DrawArgs[name].StartIndexLocation;
	moveAbleItems[code]->BaseVertexLocation = moveAbleItems[code]->Geo->DrawArgs[name].BaseVertexLocation;
	mAllRitems.push_back(std::move(moveAbleItems[code]));
}

void SoulSimul::BuildRenderItems()
{
	// (���⼭ ���ʷ� ������Ʈ �� ���ָ� ���� ��)

	// �÷��̾� ���� 
	BuildObject("player", "playerMat", "playerGeo", player[0], INDEXPLAYER);

	// ### ���� �� ���� (�ӽ�)
	BuildObject("enemy_m", "enemy_mMat", "enemy_mGeo", enemy_m, INDEXENEMY);

	// �ٴ� ���� (���⼭ �����ϰ� ������ ���� ������Ʈ���� ����)
	BuildObject("floor", "enemy_mMat", "floor_mGeo", floorobj, INDEXFLOOR);

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
