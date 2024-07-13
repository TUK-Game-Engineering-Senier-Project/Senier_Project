//***************************************************************************************
// SoulSimul.cpp by Frank Luna (C) 2015 All Rights Reserved.
//***************************************************************************************

// 기존 Client파일에서 서버 연결 제거하고 상하좌우 이동만 남겨둔 버전
// fbx sdk 다운로드시 있던 sample 폴더에 있는 humanoid.fbx 사용
// humanoid.fbx는 Client 폴더 내에 있음, 위치 변경시 변경된 위치 적용하기
// 여기 있는, 방패나 무기를 언급하는 코드는 현재 의미 없음

// Visual Studio에서 (본인 폴더 기준)
// fbx sdk 사용시 #include <fbxsdk.h>를 필수로 할 것
// - C / C++ - 일반 - 추가 포함 디렉터리 설정 : include 경로 D:\FBX_SDK\2020.3.7\include 지정
// - 속성 - 링커 - 일반 - 추가 라이브러리 디렉터리 설정 : FBX SDK 경로 D:\FBX_SDK\2020.3.7\lib\x64\debug 지정

// 추가한 것들
// void SoulSimul::BuildFbxGeometry(const char* filename, const char* subMeshName, const char* geoName, 
//     float scaleMulX, float scaleMulY, float scaleMulZ);
// void SoulSimul::BuildObject(const char* name, const char* mat, const char* geo, Object c)

// Player 클래스를 Character 클래스로 수정
// 어차피 바닥도 fbx로 불러올 것이기 때문에 
// - UpdateObject와 BuildItem을 BuildObject로 통합함
// 지금 바닥이라 되어있는 건 중앙을 표시하는 건 작은 직육면체

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

// 프레임 리소스 개수
const int gNumFrameResources = 3;

// 각 오브젝트 인덱스
constexpr char INDEXPLAYER = 0;
constexpr char INDEXFLOOR = 1;
constexpr char INDEXENEMY = 2;

// lua 사용
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

// RenderItem 모음 벡터
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

	// 오브젝트 빌드
	void BuildObject(const char* name, const char* mat, const char* geo, Object c, const int code);

	// 카메라 업데이트 (플레이어 위치 확정된 뒤)
	void UpdateCamera(const GameTimer& gt);

	void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMaterialCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);

    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildShapeGeometry();

	// FBX 빌드 (파일명, geo 이름, subMesh 이름, 불러올 때 크기 배율)
	void SoulSimul::BuildFbxGeometry(const char* filename, const char* subMeshName, const char* geoName,
		float scaleMulX, float scaleMulY, float scaleMulZ);

    void BuildPSOs();
    void BuildFrameResources();

	// 재질 build (코드 간략화)
	void BuildMaterial(char* name, int index,
		XMFLOAT4 DiffuseAlbedo, XMFLOAT3 FresnelR0, float roughness);

	// 재질 전체 빌드
	void BuildMaterials();

	// 아이템 빌드 (코드 간략화)
	void BuildItem(const char* name, const char* mat, const char* geo);

	// 아이템 전체 빌드 및 그리기
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
	// ----- Lua 열기 -----
	luaL_openlibs(L);
	luaL_loadfile(L, "LuaObject.lua");
	lua_pcall(L, 0, 0, 0);

	// ### 테스트 [출력]용 함수 lua에 등록
	// lua_registerLuaDebugOutput(L);
}

SoulSimul::~SoulSimul()
{
    if(md3dDevice != nullptr)
        FlushCommandQueue();

	// ----- lua 닫기 -----
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

	// 플레이어 오브젝트 추가 (체력, 좌표)
	lua_newObject(L, "player1", 100, -4.0f, 0.0f, -3.0f);

	// 적 오브젝트 추가 (체력, 좌표)
	lua_newObject(L, "enemy_m", 100, 5.0f, 0.0f, 2.0f);

	// 플레이어 모양 빌드 
	// 매개변수 : 파일명, subMesh 이름, geo 이름, 불러올 때 크기 배율
	BuildFbxGeometry("sample_humanoid.fbx", "player", "playerGeo", 0.017f, 0.017f, 0.017f);

	// ### 중형 적 빌드 (임시)
	BuildFbxGeometry("sample_humanoid.fbx", "enemy_m", "enemy_mGeo", 0.025f, 0.025f, 0.025f);

	// ### 바닥 빌드 (임시)
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
	lua_BehaviorTree(L, "enemy_m", "player1", player[0].pos_x, player[0].pos_z); // 행동 트리 갱신
	lua_DoAction(L, "enemy_m", "player1", player[0].pos_x, player[0].pos_z);     // 행동 트리에 따른 동작 수행

	// --- 데이터 업데이트 ---

	// 적 위치 업데이트
	enemy_m.pos_x = lua_getFuncFloat(L, "enemy_m", "GetData", "pos_x");
	enemy_m.pos_y = lua_getFuncFloat(L, "enemy_m", "GetData", "pos_y");
	enemy_m.pos_z = lua_getFuncFloat(L, "enemy_m", "GetData", "pos_z");

	// 적 회전 업데이트
	enemy_m.rotate_x = lua_getFuncFloat(L, "enemy_m", "GetData", "rotate_x");
	enemy_m.rotate_y = lua_getFuncFloat(L, "enemy_m", "GetData", "rotate_y");
	enemy_m.rotate_z = lua_getFuncFloat(L, "enemy_m", "GetData", "rotate_z");
	
	// 플레이어 위치 업데이트

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

	// --- 데이터가 업데이트된 후에 오브젝트를 빌드함 ---

	// 플레이어 빌드 
	BuildObject("player", "playerMat", "playerGeo", player[0], INDEXPLAYER);
	// ### 여기에 다른 플레이어를 추가할 것

	// ### 중형 적 빌드 (임시)
	BuildObject("enemy_m", "enemy_mMat", "enemy_mGeo", enemy_m, INDEXENEMY);
	// ### 여기에 다른 적을 추가할 것

	// 카메라 업데이트 (오브젝트들 위치 확정된 뒤)
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
    float fBehindPlayer = 12.0f; // 플레이어로부터 뒤 거리
    float fAbovePlayer  = 10.0f; // 플레이어로부터 위 거리 

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

	// 조명
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

	// 각 메쉬들의 위치를 지정

	SubmeshGeometry floorSubmesh; // 바닥
	floorSubmesh.IndexCount = (UINT)floor.Indices32.size();
	floorSubmesh.StartIndexLocation = floorIndexOffset;
	floorSubmesh.BaseVertexLocation = floorVertexOffset;
	SubmeshGeometry weaponSubmesh; // 무기
	weaponSubmesh.IndexCount = (UINT)weapon.Indices32.size();
	weaponSubmesh.StartIndexLocation = weaponIndexOffset;
	weaponSubmesh.BaseVertexLocation = weaponVertexOffset;
	SubmeshGeometry shieldSubmesh; // 방패
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

void SoulSimul::BuildFbxGeometry(const char* filename, const char* subMeshName, const char* geoName,
	float scaleMulX, float scaleMulY, float scaleMulZ)
{
	// FBX SDK 초기화
	FbxManager* sdkManager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(sdkManager, IOSROOT);
	sdkManager->SetIOSettings(ios);

	// importer 생성
	FbxImporter* importer = FbxImporter::Create(sdkManager, "");

	// importer 초기화 오류시 종료
	if (!importer->Initialize(filename, -1, sdkManager->GetIOSettings()))
	{
		importer->Destroy();
		sdkManager->Destroy();
		return;
	}

	// 불러온 데이터를 저장할 scene 만들기
	FbxScene* scene = FbxScene::Create(sdkManager, "Scene");

	// scene import하기
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

	// mesh에서 vertex 및 index 데이터 추출
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
		// 크기 배율 조정
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

	// vertex 버퍼 및 index 버퍼 생성하고 계산 처리
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

	// 플레이어 SubMesh 생성 및 지정
	SubmeshGeometry subMesh;
	subMesh.IndexCount = static_cast<UINT>(indexData.size());
	subMesh.StartIndexLocation = 0;
	subMesh.BaseVertexLocation = 0;
	geo->DrawArgs[subMeshName] = subMesh;

	mGeometries[geo->Name] = std::move(geo);

	// scene과 sdkManager 데이터 정리
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

// 재질 전체 build
void SoulSimul::BuildMaterials()
{
	// 바닥 재질
	BuildMaterial
	(
		"floorMat", INDEXFLOOR,             // material 이름 및 인덱스
		XMFLOAT4(0.95f, 0.80f, 0.0f, 1.0f), // DiffuseAlbedo
		XMFLOAT3(0.02f, 0.02f, 0.02f), 0.2f // FresnelR0, Roughness
	);

	// 플레이어 재질
	BuildMaterial
	(
		"playerMat", INDEXPLAYER,           // material 이름 및 인덱스
		XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f),   // DiffuseAlbedo
		XMFLOAT3(0.05f, 0.05f, 0.05f), 0.3f // FresnelR0, Roughness
	);

	// ### 중형 적 재질 (임시)
	BuildMaterial
	(
		"enemy_mMat", INDEXENEMY,           // material 이름 및 인덱스
		XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f),   // DiffuseAlbedo
		XMFLOAT3(0.05f, 0.05f, 0.05f), 0.4f // FresnelR0, Roughness
	);

	// ### 필요시 재질 추가하기 (BuildMaterial())
}

// 오브젝트 빌드
// Character 클래스 값을 입력받는 이유는 
// - 해당 클래스에서 배율, 회전, 위치 값을 받아 사용하기 때문이다.

void SoulSimul::BuildObject(const char* name, const char* mat, const char* geo, Object obj, const int code)
{
	// objcode는 BuildObject 각 객체를 구분하기 위한 int형 코드

	// 크기 조정
	if (code >= moveAbleItems.size()) {
		moveAbleItems.resize(code + 1);
	}

	// RenderItem 아이템 변수
	moveAbleItems[code] = std::make_unique<RenderItem>();

	// 오브젝트 갱신
	XMStoreFloat4x4
	(
		// 불러온 Character c에 해당하는 배율, 회전, 위치에 맞춰 지정
		&moveAbleItems[code]->World,
		XMMatrixScaling(obj.scale_x, obj.scale_y, obj.scale_z)
		* XMMatrixRotationRollPitchYaw(obj.rotate_x, obj.rotate_y, obj.rotate_z)
		* XMMatrixTranslation(obj.pos_x, obj.pos_y, obj.pos_z)
	);

	// BuildItem 코드와 거의 동일

	moveAbleItems[code]->TexTransform = MathHelper::Identity4x4();
	moveAbleItems[code]->ObjCBIndex = code; // 여기서 code는 각 오브젝트에 따라 다름
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
	// (여기서 최초로 업데이트 안 해주면 오류 남)

	// 플레이어 빌드 
	BuildObject("player", "playerMat", "playerGeo", player[0], INDEXPLAYER);

	// ### 중형 적 빌드 (임시)
	BuildObject("enemy_m", "enemy_mMat", "enemy_mGeo", enemy_m, INDEXENEMY);

	// 바닥 빌드 (여기서 유일하게 빌드한 다음 업데이트하지 않음)
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
