#pragma once

// ----- ������Ʈ ���� ���� ��� ���� -----

#include "Stdafx.h" // ����� ���� ���

#include "../Common/GameApp.h"
#include "../Common/MathHelper.h"
#include "../Common/UploadBuffer.h"


using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

// ����
struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

// ������Ʈ ���
struct ObjectConstants
{
    XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
};

// ���α׷� ����
class SoulSimul : public GameApp
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

    void BuildDescriptorHeaps();
    void BuildConstantBuffers();
    void BuildRootSignature();
    void BuildShadersAndInputLayout();

    void BuildPlayerGeometry(); // �÷��̾�
    void BuildPlayerLookGeometry(); // �÷��̾� �ü�

    void BuildPSO();

private:

    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
    
    ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;
    ComPtr<ID3D12DescriptorHeap> mCbvHeap2 = nullptr;

    std::unique_ptr<MeshGeometry> mPlayer = nullptr; // �÷��̾�
    std::unique_ptr<MeshGeometry> mPlayerLook = nullptr; // �÷��̾� �ü�

    std::unique_ptr<UploadBuffer<ObjectConstants>> mPlayerCB = nullptr;
    std::unique_ptr<UploadBuffer<ObjectConstants>> mPlayerLookCB = nullptr;



    ComPtr<ID3DBlob> mvsByteCode = nullptr;
    ComPtr<ID3DBlob> mpsByteCode = nullptr;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    ComPtr<ID3D12PipelineState> mPSO = nullptr;

    XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
    XMFLOAT4X4 mView = MathHelper::Identity4x4();
    XMFLOAT4X4 mProj = MathHelper::Identity4x4();

    float mTheta = 1.5f * XM_PI;
    float mPhi = XM_PIDIV4;
    float mRadius = 10.0f;

    POINT mLastMousePos;
};
