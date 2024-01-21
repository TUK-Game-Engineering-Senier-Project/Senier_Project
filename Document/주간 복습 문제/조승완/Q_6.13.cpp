// ---[1�� ����]---

/*
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Tangent;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex0;
	XMFLOAT2 Tex1;
	XMCOLOR Color;
};

const int ciXMF2 = 8;  // XMFLOAT2 Size
const int ciXMF3 = 12; // XMFLOAT3 Size
const int ciXMFC = 16; // XMCOLOR Size

D3D12_INPUT_ELEMENT_DESC vertexDesc[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	{"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, (ciXMF3 * 1),
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, (ciXMF3 * 2),
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, (ciXMF3 * 3),
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, (ciXMF3 * 3 + ciXMF2 * 1),
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, (ciXMF3 * 3 + ciXMF2 * 2),
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
};
*/

// ---[2�� ����]---

/*
struct VposData { XMFLOAT3 Pos; };
struct VColorData { XMFLOAT4 Color; };

struct Vertex
{
    VposData PosData;
    VColorData ColorData;
};

D3D12_INPUT_ELEMENT_DESC vertexDesc[] =
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
};
*/

// ---[3�� ����]---

/*
// Q_6.13.3 (a) �� ���
mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

// Q_6.13.3 (b) �� ��
mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

// Q_6.13.3 (c) �� ���
mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

// Q_6.13.3 (d) �ﰢ�� ��
mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

// Q_6.13.3 (e) �ﰢ�� ���
mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
*/

// ---[4�� ����]---

/*
 std::array<Vertex, 5> vertices =
	{
		Vertex({ XMFLOAT3(+0.0f, +1.0f, +0.0f), XMFLOAT4(Colors::Red) }), // ������
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
	};

	std::array<std::uint16_t, 18> indices =
	{
		// �ٴ�
		1, 2, 3,
		1, 3, 4,

		// ����
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 1, 4,
	};
*/

// ---[5�� ����]---

// �ﰢ���� �� ������ ������ �����Ͽ� �� ���̿� �ִ� �ȼ��� �����ϱ� �����̴�.

// ---[6�� ����]---

/*
// BoxApp::Update�� view matrix ���� ������ �ڵ� �߰�

float gTime = gt.TotalTime();
x += 0.5f * sinf(x) * sinf(3.0f * gTime);
y += 0.5f * sinf(x) * sinf(3.0f * gTime);
z *= 0.6f + 0.4f * sinf(2.0f * gTime);
*/

// ---[7�� ����]---

/*

// global

// ������ v, i ����
const int boxVnum = 8; // ����ü vertex ����
const int boxInum = 36; // ����ü indice ����
const int sqpyramidVnum = 5; // �簢�� vertex ����
const int sqpyramidInum = 18; // �簢�� indice ����

// void BoxApp::BuildBoxGeometry()

std::array<Vertex, 13> vertices =
{
    // ����ü
    Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
    Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
    Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
    Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
    Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
    Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
    Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
    Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) }),

    // �簢��
    Vertex({ XMFLOAT3(+4.0f, +2.0f, +0.0f), XMFLOAT4(Colors::Red) }), // ������
    Vertex({ XMFLOAT3(+2.8f, -1.0f, -1.2f), XMFLOAT4(Colors::Green) }),
    Vertex({ XMFLOAT3(+2.8f, -1.0f, +1.2f), XMFLOAT4(Colors::Green) }),
    Vertex({ XMFLOAT3(+5.2f, -1.0f, +1.2f), XMFLOAT4(Colors::Green) }),
    Vertex({ XMFLOAT3(+5.2f, -1.0f, -1.2f), XMFLOAT4(Colors::Green) }),
};

std::array<std::uint16_t, 54> indices =
{
    // ����ü

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
    4, 3, 7,

    // �簢��

    // �ٴ�
    1, 2, 3,
    1, 3, 4,

    // ����
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 1, 4,
};

// ����ü ����
SubmeshGeometry SMGBox;
SMGBox.IndexCount = boxInum;
SMGBox.StartIndexLocation = 0;
SMGBox.BaseVertexLocation = 0;

// �簢�� ����
SubmeshGeometry SMGPyramid;
SMGPyramid.IndexCount = sqpyramidInum;
SMGPyramid.StartIndexLocation = boxInum;
SMGPyramid.BaseVertexLocation = boxVnum;

mBoxGeo->DrawArgs["box"] = SMGBox;
mBoxGeo->DrawArgs["pyramid"] = SMGPyramid;

// BoxApp::Draw()

// ����ü, �簢�� �׸���

mCommandList->DrawIndexedInstanced(
	mBoxGeo->DrawArgs["box"].IndexCount,
	1, 0, 0, 0);

mCommandList->DrawIndexedInstanced(
	mBoxGeo->DrawArgs["pyramid"].IndexCount,
	1,
	boxInum, // ����ü�� �ش��ϴ� index �������� �׸���
	boxVnum, // ����ü�� �ش��ϴ� vertex �������� �׸���
	0);

*/

// ---[8�� ����]---

/*
// BoxApp::BuildPSO() ����
// ������ȭ�� ���� ����
CD3DX12_RASTERIZER_DESC rsDesc(D3D12_DEFAULT);
rsDesc.FillMode = D3D12_FILL_MODE_WIREFRAME; // ���̾������� ���
psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(rsDesc);
*.


// ---[9�� ����]---

/*
// BoxApp::BuildPSO() ����
// ������ȭ�� ���� ����
CD3DX12_RASTERIZER_DESC rsDesc(D3D12_DEFAULT);
rsDesc.FillMode = D3D12_FILL_MODE_WIREFRAME; // ���̾������� ���
rsDesc.CullMode = D3D12_CULL_MODE_NONE; // ����(�ø�) ���� (���� : _FRONT / �ĸ� : _BACK, ���� : _NONE)
psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(rsDesc);

// -> BACK�� �����ϸ� �ո� �ﰢ������, FRONT�� �����ϸ� �޸� �ﰢ������ ���δ�.
// -> NONE���� �����ϸ� ��� �ﰢ������ ���δ�.
*/

// ---[10�� ����]---

/*
D3D12_INPUT_ELEMENT_DESC vertexDesc[] =
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0, 0,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
};
*/

// ---[11�� ����]---

/*
(a)
���� : ���������� ����� ���̴�.
���� : {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}����
5��° �Ű������� ���� ��ġ�� �����ϱ� ������ �ڵ� ������ �ٲ���� ��ġ���� ������ ���� ���̴�.

BoxApp.cpp���� �ڵ带
D3D12_INPUT_ELEMENT_DESC vertexDesc[] =
{
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
};
�� �ٲ� �����غ��Ҵ�.
> ��� : ������ ���� ����Ǿ���.

(b)
���� : (a)���� �׷��� ���� ��ġ�� �����Ǿ� �ֱ� ������ ������ ������� (���� �����) ���̴�.
���� : ����

color.hlsl���� �ڵ带 
struct VertexIn
{
    float4 Color : COLOR;
    float3 PosL : POSITION;
};
�� �ٲ� �����غ��Ҵ�.
> ��� : ������ ���� ����Ǿ���.
*/

// ---[12�� ����]---

/*
mCommandList->RSSetViewports(1, &mScreenViewport); ��

D3D12_VIEWPORT leftViewport = { 0.0f, 0.0f, static_cast<float>(mClientWidth) / 2.0f, static_cast<float>(mClientHeight), 0.0f, 1.0f };
mCommandList->RSSetViewports(1, &leftViewport);
�� ��ȯ�Ͽ� ���� ���� �������� ǥ���ϵ��� �Ͽ���.
*/

// ---[13�� ����]---

/*
mCommandList->RSSetScissorRects(1, &mScissorRect); ��

D3D12_RECT scissorRect = { static_cast<LONG>(mClientWidth) * 1 / 4, static_cast<LONG>(mClientHeight) * 1 / 4,
    static_cast<LONG>(mClientWidth) * 3 / 4, static_cast<LONG>(mClientHeight) * 3 / 4 };
mCommandList->RSSetScissorRects(1, &scissorRect);
�� ��ȯ�Ͽ� ��� ���� �������� ǥ���ϵ��� �Ͽ���. 
����/���ʿ��� 1/4 ��ġ���� ����/���ʿ��� 3/4���� ��ġ�� ǥ���Ѵ�.
*/

// ---[14�� ����]---

/*

// [���� ���̴�����]

// color.hlsl

// cbuffer cbPerObject : register(b0)
float gTime;

// VertexOut VS(VertexIn vin)

const float pi = 3.14159; // ������ ���

float colorChange = 0.5f * sin(2 * gTime - 0.25f * pi) + 0.5f; // ���� ��ȭ
float4 changedColor4 = { colorChange, colorChange, colorChange, 0.0f }; // �ٲ� ���� ����
float4 result = lerp(vin.Color, changedColor4, colorChange); // ���� ���� 

// ����� ��� ���� �����Ѵ�
vout.Color = result;
return vout;

// BoxApp::Update()

// �ð� ����
mTime += static_cast<float>(gt.DeltaTime());
if (mTime > 1.0f) mTime = 1.0f;

// ��� ���ۿ� �ð� ����
objConstants.gTime = mTime;

// ���̴��� ������ ����
mObjectCB->CopyData(0, objConstants);

// [�ȼ� ���̴�����]

// VertexOut VS(VertexIn vin) �� �����·� ��������
// float4 PS(VertexOut pin) : SV_Target ����
{
    const float pi = 3.14159; // ������ ���

    float colorChange = 0.5f * sin(2 * gTime - 0.25f * pi) + 0.5f; // ���� ��ȭ
    float4 changedColor4 = { colorChange, colorChange, colorChange, 0.0f }; // �ٲ� ���� ����
    float4 result = lerp(pin.Color, changedColor4, colorChange); // ���� ����

    // ����� ��� ���� �����Ѵ�
    return result;
}

*/

// ---[15�� ����]---

/*
���� ����� ���� �� clip(pin.Color.r - 0.5f);��
"�� �������κ��� ���� ������ ���� �κ��� �߶󳽴�." ����� �����ϴ� �� ���δ�.
*/

// ---[16�� ����]---

/*
struct ObjectConstants
{
    XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
    XMFLOAT4 objPulseColor = { 0.0f, 0.0f, 0.0f, 0.0f }; // ������Ʈ ����
    float objTime = 0.0f; // ������Ʈ �ð�
};

// void BoxApp::Update(const GameTimer& gt)��

// �ð� ����
float mTime = gt.TotalTime();

// ��� ���ۿ� �ð� ����
objConstants.objTime = mTime;

// ��� ���ۿ� ���� ����
objConstants.objPulseColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // �ٲ� ���� ����

// ���̴��� ������ ����
mObjectCB->CopyData(0, objConstants);

// color.hlsl
float4 PS(VertexOut pin) : SV_Target
{
    const float pi = 3.14159; // ������ ���

    float colorChange = 0.5f * sin(2 * gTime - 0.25f * pi) + 0.5f; // ���� ��ȭ
    float4 result = lerp(pin.Color, gPulseColor, colorChange); // ���� ���� 

    // ����� ��� ���� �����Ѵ�
    return result;
}
*/