// 게임 객체의 정보를 위한 상수 버퍼를 선언한다.
cbuffer cbGameObjectInfo : register(b0)
{
    matrix gmtxWorld : packoffset(c0);
};
 
// 카메라의 정보를 위한 상수 버퍼를 선언한다.
cbuffer cbCameraInfo : register(b1)
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
};

// 정점 셰이더의 입력을 위한 구조체를 선언한다.
struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

// 정점 셰이더의 출력(픽셀 셰이더의 입력)을 위한 구조체를 선언한다.
struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

// 정점 셰이더를 정의한다.
VS_OUTPUT VSDiffused(VS_INPUT input)
{
    VS_OUTPUT output;
    // 정점을 변환(월드 변환, 카메라 변환, 투영 변환)한다.
    output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView),
    gmtxProjection);
    output.color = input.color;
    return (output);
}

// 픽셀 셰이더를 정의한다.
float4 PSDiffused(VS_OUTPUT input) : SV_TARGET
{
    return (input.color);
}

// 인스턴싱 데이터를 위한 구조체이다.
struct INSTANCEDGAMEOBJECTINFO
{
    matrix m_mtxGameObject;
    float4 m_cColor;
};

StructuredBuffer<INSTANCEDGAMEOBJECTINFO> gGameObjectInfos : register(t0);

struct VS_INSTANCING_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_INSTANCING_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_INSTANCING_OUTPUT VSInstancing(VS_INSTANCING_INPUT input, uint nInstanceID : SV_InstanceID)
{
    VS_INSTANCING_OUTPUT output;
    output.position = mul(mul(mul(float4(input.position, 1.0f),
    gGameObjectInfos[nInstanceID].m_mtxGameObject), gmtxView), gmtxProjection);
    output.color = input.color + gGameObjectInfos[nInstanceID].m_cColor;
    return (output);
}

float4 PSInstancing(VS_INSTANCING_OUTPUT input) : SV_TARGET
{
    return (input.color);
}


// ----- <6-1> 내용 ----- //

/*
// 정점 쉐이더를 정의한다. 렌더 타겟 사각형 전체를 삼각형 2개로 표현한다.
float4 VSMain(uint nVertexID : SV_VertexID) : SV_POSITION
{
    float4 output = (float4) 0;
    if (nVertexID == 0)
        output = float4(-1.0f, +1.0f, 0.0f, 1.0f);
    else if (nVertexID == 1)
        output = float4(+1.0f, +1.0f, 0.0f, 1.0f);
    else if (nVertexID == 2)
        output = float4(+1.0f, -1.0f, 0.0f, 1.0f);
    else if (nVertexID == 3)
        output = float4(-1.0f, +1.0f, 0.0f, 1.0f);
    else if (nVertexID == 4)
        output = float4(+1.0f, -1.0f, 0.0f, 1.0f);
    else if (nVertexID == 5)
        output = float4(-1.0f, -1.0f, 0.0f, 1.0f);
    return (output);
}
*/

// #define FRAME_BUFFER_WIDTH  640.0f
// #define FRAME_BUFFER_HEIGHT 480.0f

// 픽셀 쉐이더를 다음과 같이 정의하여 실행해 보면 다음과 같은 결과를 볼 수 있다.
/*
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    cColor.r = input.x /  FRAME_BUFFER_WIDTH;
    return (cColor);
}
*/

// 픽셀 쉐이더를 다음과 같이 정의하여 실행해 보면 다음과 같은 결과를 볼 수 있다.
/*
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    cColor.r = input.x / FRAME_BUFFER_WIDTH;
    cColor.g = input.y / FRAME_BUFFER_HEIGHT;
    return (cColor);
}
*/

// 픽셀 쉐이더를 다음과 같이 정의하여 실행해 보면 다음과 같은 결과를 볼 수 있다.
/*
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
 // cColor.rgb = distance(float2(0.0f, 0.0f), input.xy / float2(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT));
 // cColor.rgb = length(input.xy / float2(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT));
 // cColor.rgb = distance(float2(0.0f, 0.0f), (input.xy / float2(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT)-0.5f)); // (0, 1) : (-0.5, 0.5)
 // cColor.rgb = length(input.xy / float2(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT) - 0.5f);
    cColor.rgb = distance(float2(0.5f, 0.5f), input.xy / float2(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT));
    return (cColor);
}
*/

// 픽셀 쉐이더를 다음과 같이 정의하여 실행해 보면 다음과 같은 결과를 볼 수 있다.
/*
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float fDistance = distance(float2(0.5f, 0.5f), input.xy / float2(FRAME_BUFFER_WIDTH,FRAME_BUFFER_HEIGHT));
    if (fDistance < 0.25f)
        cColor.b = 1.0f;
    return (cColor);
}
*/

// 픽셀 쉐이더를 다음과 같이 정의하여 실행해 보면 다음과 같은 결과를 볼 수 있다
/*
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float2 f2NDC = float2(input.x / FRAME_BUFFER_WIDTH, input.y / FRAME_BUFFER_HEIGHT) - 0.5f; // (0, 1) : (-0.5, 0.5)
    f2NDC.x *= (FRAME_BUFFER_WIDTH / FRAME_BUFFER_HEIGHT);
    // cColor.b = step(length(f2NDC), 0.25f); // step(x, y) = (x <= y) ? 1 : 0
    cColor.b = (length(f2NDC) <= 0.25f) ? 1.0f : 0.0f; // step(x, y) = (x <= y) ? 1 : 0
    return (cColor);
}
*/

// 픽셀 쉐이더를 다음과 같이 정의하여 실행해 보면 다음과 같은 결과를 볼 수 있다.
/*
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float2 f2NDC = float2(input.x / FRAME_BUFFER_WIDTH, input.y / FRAME_BUFFER_HEIGHT) - 0.5f;
    f2NDC.x *= (FRAME_BUFFER_WIDTH / FRAME_BUFFER_HEIGHT);
    float fLength = length(f2NDC);
    float fMin = 0.3f, fMax = 0.2f;
    cColor.rgb = smoothstep(fMin, fMax, fLength);
    return (cColor);
}
*/

// #define HALF_WIDTH (FRAME_BUFFER_WIDTH * 0.5f)
// #define HALF_HEIGHT (FRAME_BUFFER_HEIGHT * 0.5f)
// #define EPSILON 1.0e-5f
 
/*
inline bool IsZero(float fValue)
{
    return ((abs(fValue) <=  EPSILON));
}
inline bool IsZero(float fValue, float fEpsilon)
{
    return ((abs(fValue) <= fEpsilon));
}
inline bool IsEqual(float fA, float fB, float fEpsilon)
{
    return ((abs(fA - fB) <= fEpsilon));
}
*/

// 픽셀 쉐이더를 다음과 같이 정의하여 실행해 보면 다음과 같은 결과를 볼 수 있다.
/*
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    if ((int) input.x == (int) HALF_WIDTH)
        cColor.g = 1.0f;
    if ((int) input.y == (int) HALF_HEIGHT)
        cColor.r = 1.0f;
    float fDistance = distance((int2) input.xy, float2( HALF_WIDTH,  HALF_HEIGHT));
    if (IsEqual(fDistance, 100.0f, 0.5f))
        cColor.b = 1.0f;
    return (cColor);
}
*/

/*
float Rectangle(float2 f2NDC, float fLeft, float fRight, float fTop, float fBottom)
{
    float2 f2Shape = float2(step(fLeft, f2NDC.x), step(f2NDC.x, fRight));
    f2Shape *= float2(step(fTop, f2NDC.y), step(f2NDC.y, fBottom));
    return (f2Shape.x * f2Shape.y);
}
*/

// 픽셀 쉐이더를 다음과 같이 정의하여 실행해 보면 다음과 같은 결과를 볼 수 있다.
/*
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float2 f2NDC = float2(input.x / FRAME_BUFFER_WIDTH, input.y / FRAME_BUFFER_HEIGHT) - 0.5f; // (0, 1) : (-0.5, 0.5)
    f2NDC.x *= (FRAME_BUFFER_WIDTH / FRAME_BUFFER_HEIGHT);
    float2 f2Horizontal = float2(0.1f, +0.3f); //(Left, Right)
    float2 f2Vertical = float2(-0.3f, +0.3f); //(Top, Bottom)
    cColor.b = Rectangle(f2NDC, +0.1f, +0.3f, -0.2f, +0.4f);
    cColor.b += Rectangle(f2NDC, -0.3f, -0.1f, -0.4f, -0.1f);
    return (cColor);
}
*/

// 픽셀 쉐이더를 다음과 같이 정의하여 실행해 보면 다음과 같은 결과를 볼 수 있다.
/*
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float2 f2NDC = float2(input.x - FRAME_BUFFER_WIDTH * 0.5f, input.y - FRAME_BUFFER_HEIGHT * 0.5f);
    f2NDC *= 20.0f;
    float fLength = length(f2NDC);
    cColor.rgb = cos(fLength);
    return (cColor);
}
*/

// 픽셀 쉐이더를 다음과 같이 정의하여 실행해 보면 다음과 같은 결과를 볼 수 있다.
/*
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float2 f2NDC = float2(input.x / FRAME_BUFFER_WIDTH, input.y / FRAME_BUFFER_HEIGHT) - 0.5f; // (0, 1) : (-0.5, 0.5)
    f2NDC.x *= (FRAME_BUFFER_WIDTH / FRAME_BUFFER_HEIGHT);
    float fRadius = 0.3f;
    float fRadian = radians(360.0f / 30.0f);
    for (float f = 0; f < 30.0f; f += 1.0f)
    {
        float fAngle = fRadian * f;
        cColor.rgb += (0.0025f / length(f2NDC + float2(fRadius * cos(fAngle), fRadius * sin(fAngle))));
    }
    return (cColor);
}
*/

/*
float RegularPolygon(float2 f2NDC, float fSides, float fRadius)
{
    float fAngle = atan(f2NDC.y / f2NDC.x);
    float fSlices = (2.0f * 3.14159f) / fSides;
    float fShape = step(cos(floor((fAngle / fSlices) + 0.5f) * fSlices - fAngle) * length(f2NDC), fRadius);
    return (fShape);
}
*/

// 픽셀 쉐이더를 다음과 같이 정의하여 실행해 보면 다음과 같은 결과를 볼 수 있다.
/*
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float2 f2NDC = float2(input.x / FRAME_BUFFER_WIDTH, input.y / FRAME_BUFFER_HEIGHT) - 0.5f; // (0, 1) : (-0.5, 0.5)
    f2NDC.x *= (FRAME_BUFFER_WIDTH / FRAME_BUFFER_HEIGHT);
    cColor.b = RegularPolygon(f2NDC - float2(-0.3f, -0.1f), 8.0f, 0.2f); //4, 6, 8, ...
    cColor.r = RegularPolygon(f2NDC - float2(+0.3f, +0.2f), 4.0f, 0.15f);
    return (cColor);
}
*/

// 픽셀 쉐이더를 다음과 같이 정의하여 실행해 보면 다음과 같은 결과를 볼 수 있다.
/*
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float x = abs(frac((input.x * 10.0f) / FRAME_BUFFER_HEIGHT) - 0.5f);
    float y = abs(frac((input.y * 10.0f) / FRAME_BUFFER_HEIGHT) - 0.5f);
    cColor.r = ((x <= 0.0125f) || (y <= 0.0125f)) ? 1.0f : 0.0f;
    return (cColor);
}
*/

/*
// 픽셀 쉐이더를 다음과 같이 정의하여 실행해 보면 다음과 같은 결과를 볼 수 있다.
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float2 f2NDC = input.xy / float2(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT) - 0.5f;
    f2NDC.x *= (FRAME_BUFFER_WIDTH / FRAME_BUFFER_HEIGHT);
    f2NDC.xy *= 10.0f;
    cColor.b = smoothstep(0.125f, 0.875f, abs(cos(length(f2NDC) * 3.14159f)));
    return (cColor);
}
*/