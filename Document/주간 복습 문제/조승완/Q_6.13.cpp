// ---[1번 문제]---

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

// ---[2번 문제]---

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

// ---[3번 문제]---

/*
// Q_6.13.3 (a) 점 목록
mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

// Q_6.13.3 (b) 선 띠
mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

// Q_6.13.3 (c) 선 목록
mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

// Q_6.13.3 (d) 삼각형 띠
mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

// Q_6.13.3 (e) 삼각형 목록
mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
*/

// ---[4번 문제]---

/*
 std::array<Vertex, 5> vertices =
	{
		Vertex({ XMFLOAT3(+0.0f, +1.0f, +0.0f), XMFLOAT4(Colors::Red) }), // 꼭짓점
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
	};

	std::array<std::uint16_t, 18> indices =
	{
		// 바닥
		1, 2, 3,
		1, 3, 4,

		// 옆면
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 1, 4,
	};
*/

// ---[5번 문제]---

// 삼각형의 세 정점의 색상을 보간하여 점 사이에 있는 픽셀에 적용하기 때문이다.

// ---[6번 문제]---

/*
// BoxApp::Update의 view matrix 빌드 이전에 코드 추가

float gTime = gt.TotalTime();
x += 0.5f * sinf(x) * sinf(3.0f * gTime);
y += 0.5f * sinf(x) * sinf(3.0f * gTime);
z *= 0.6f + 0.4f * sinf(2.0f * gTime);
*/

// ---[7번 문제]---

/*

// global

// 도형별 v, i 개수
const int boxVnum = 8; // 육면체 vertex 개수
const int boxInum = 36; // 육면체 indice 개수
const int sqpyramidVnum = 5; // 사각뿔 vertex 개수
const int sqpyramidInum = 18; // 사각뿔 indice 개수

// void BoxApp::BuildBoxGeometry()

std::array<Vertex, 13> vertices =
{
    // 육면체
    Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
    Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
    Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
    Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
    Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
    Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
    Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
    Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) }),

    // 사각뿔
    Vertex({ XMFLOAT3(+4.0f, +2.0f, +0.0f), XMFLOAT4(Colors::Red) }), // 꼭짓점
    Vertex({ XMFLOAT3(+2.8f, -1.0f, -1.2f), XMFLOAT4(Colors::Green) }),
    Vertex({ XMFLOAT3(+2.8f, -1.0f, +1.2f), XMFLOAT4(Colors::Green) }),
    Vertex({ XMFLOAT3(+5.2f, -1.0f, +1.2f), XMFLOAT4(Colors::Green) }),
    Vertex({ XMFLOAT3(+5.2f, -1.0f, -1.2f), XMFLOAT4(Colors::Green) }),
};

std::array<std::uint16_t, 54> indices =
{
    // 육면체

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

    // 사각뿔

    // 바닥
    1, 2, 3,
    1, 3, 4,

    // 옆면
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 1, 4,
};

// 육면체 도형
SubmeshGeometry SMGBox;
SMGBox.IndexCount = boxInum;
SMGBox.StartIndexLocation = 0;
SMGBox.BaseVertexLocation = 0;

// 사각뿔 도형
SubmeshGeometry SMGPyramid;
SMGPyramid.IndexCount = sqpyramidInum;
SMGPyramid.StartIndexLocation = boxInum;
SMGPyramid.BaseVertexLocation = boxVnum;

mBoxGeo->DrawArgs["box"] = SMGBox;
mBoxGeo->DrawArgs["pyramid"] = SMGPyramid;

// BoxApp::Draw()

// 육면체, 사각뿔 그리기

mCommandList->DrawIndexedInstanced(
	mBoxGeo->DrawArgs["box"].IndexCount,
	1, 0, 0, 0);

mCommandList->DrawIndexedInstanced(
	mBoxGeo->DrawArgs["pyramid"].IndexCount,
	1,
	boxInum, // 육면체에 해당하는 index 다음부터 그린다
	boxVnum, // 육면체에 해당하는 vertex 다음부터 그린다
	0);

*/

// ---[8번 문제]---

/*
// BoxApp::BuildPSO() 내부
// 레스터화기 상태 설정
CD3DX12_RASTERIZER_DESC rsDesc(D3D12_DEFAULT);
rsDesc.FillMode = D3D12_FILL_MODE_WIREFRAME; // 와이어프레임 모드
psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(rsDesc);
*.


// ---[9번 문제]---

/*
// BoxApp::BuildPSO() 내부
// 레스터화기 상태 설정
CD3DX12_RASTERIZER_DESC rsDesc(D3D12_DEFAULT);
rsDesc.FillMode = D3D12_FILL_MODE_WIREFRAME; // 와이어프레임 모드
rsDesc.CullMode = D3D12_CULL_MODE_NONE; // 선별(컬링) 여부 (전면 : _FRONT / 후면 : _BACK, 없음 : _NONE)
psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(rsDesc);

// -> BACK로 설정하면 앞면 삼각형들이, FRONT로 설정하면 뒷면 삼각형들이 보인다.
// -> NONE으로 설정하면 모든 삼각형들이 보인다.
*/

// ---[10번 문제]---

/*
D3D12_INPUT_ELEMENT_DESC vertexDesc[] =
{
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    {"COLOR", 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0, 0,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
};
*/

// ---[11번 문제]---

/*
(a)
추측 : 정상적으로 실행될 것이다.
이유 : {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}에서
5번째 매개변수가 시작 위치를 지정하기 때문에 코드 순서가 바뀌더라도 위치에는 문제가 없을 것이다.

BoxApp.cpp에서 코드를
D3D12_INPUT_ELEMENT_DESC vertexDesc[] =
{
    {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
        D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
};
로 바꿔 실행해보았다.
> 결과 : 예상대로 정상 실행되었다.

(b)
추측 : (a)에서 그랬듯 시작 위치가 지정되어 있기 때문에 순서는 상관없을 (정상 실행될) 것이다.
이유 : 동일

color.hlsl에서 코드를 
struct VertexIn
{
    float4 Color : COLOR;
    float3 PosL : POSITION;
};
로 바꿔 실행해보았다.
> 결과 : 예상대로 정상 실행되었다.
*/

// ---[12번 문제]---

/*
mCommandList->RSSetViewports(1, &mScreenViewport); 를

D3D12_VIEWPORT leftViewport = { 0.0f, 0.0f, static_cast<float>(mClientWidth) / 2.0f, static_cast<float>(mClientHeight), 0.0f, 1.0f };
mCommandList->RSSetViewports(1, &leftViewport);
로 변환하여 왼쪽 절반 범위만을 표시하도록 하였다.
*/

// ---[13번 문제]---

/*
mCommandList->RSSetScissorRects(1, &mScissorRect); 를

D3D12_RECT scissorRect = { static_cast<LONG>(mClientWidth) * 1 / 4, static_cast<LONG>(mClientHeight) * 1 / 4,
    static_cast<LONG>(mClientWidth) * 3 / 4, static_cast<LONG>(mClientHeight) * 3 / 4 };
mCommandList->RSSetScissorRects(1, &scissorRect);
로 변환하여 가운데 절반 범위만을 표시하도록 하였다. 
왼쪽/위쪽에서 1/4 위치부터 왼쪽/위쪽에서 3/4까지 위치를 표시한다.
*/

// ---[14번 문제]---

/*

// [정점 셰이더에서]

// color.hlsl

// cbuffer cbPerObject : register(b0)
float gTime;

// VertexOut VS(VertexIn vin)

const float pi = 3.14159; // 원주율 상수

float colorChange = 0.5f * sin(2 * gTime - 0.25f * pi) + 0.5f; // 색상 변화
float4 changedColor4 = { colorChange, colorChange, colorChange, 0.0f }; // 바뀐 색상 적용
float4 result = lerp(vin.Color, changedColor4, colorChange); // 선형 보간 

// 결과를 출력 색상에 적용한다
vout.Color = result;
return vout;

// BoxApp::Update()

// 시간 갱신
mTime += static_cast<float>(gt.DeltaTime());
if (mTime > 1.0f) mTime = 1.0f;

// 상수 버퍼에 시간 갱신
objConstants.gTime = mTime;

// 셰이더에 데이터 복사
mObjectCB->CopyData(0, objConstants);

// [픽셀 셰이더에서]

// VertexOut VS(VertexIn vin) 는 원상태로 돌려놓고
// float4 PS(VertexOut pin) : SV_Target 에서
{
    const float pi = 3.14159; // 원주율 상수

    float colorChange = 0.5f * sin(2 * gTime - 0.25f * pi) + 0.5f; // 색상 변화
    float4 changedColor4 = { colorChange, colorChange, colorChange, 0.0f }; // 바뀐 색상 적용
    float4 result = lerp(pin.Color, changedColor4, colorChange); // 선형 보간

    // 결과를 출력 색상에 적용한다
    return result;
}

*/

// ---[15번 문제]---

/*
실행 결과로 봤을 때 clip(pin.Color.r - 0.5f);는
"점 색상으로부터 절반 이후의 색상 부분은 잘라낸다." 기능을 수행하는 듯 보인다.
*/

// ---[16번 문제]---

/*
struct ObjectConstants
{
    XMFLOAT4X4 WorldViewProj = MathHelper::Identity4x4();
    XMFLOAT4 objPulseColor = { 0.0f, 0.0f, 0.0f, 0.0f }; // 오브젝트 색상
    float objTime = 0.0f; // 오브젝트 시간
};

// void BoxApp::Update(const GameTimer& gt)에

// 시간 갱신
float mTime = gt.TotalTime();

// 상수 버퍼에 시간 갱신
objConstants.objTime = mTime;

// 상수 버퍼에 색상 갱신
objConstants.objPulseColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // 바뀐 색상 적용

// 셰이더에 데이터 복사
mObjectCB->CopyData(0, objConstants);

// color.hlsl
float4 PS(VertexOut pin) : SV_Target
{
    const float pi = 3.14159; // 원주율 상수

    float colorChange = 0.5f * sin(2 * gTime - 0.25f * pi) + 0.5f; // 색상 변화
    float4 result = lerp(pin.Color, gPulseColor, colorChange); // 선형 보간 

    // 결과를 출력 색상에 적용한다
    return result;
}
*/