#include "Light.hlsl"

//�÷��̾� ��ü�� �����͸� ���� ��� ����
cbuffer cbPlayerInfo : register(b0)
{
    matrix gmtxPlayerWorld : packoffset(c0);
}

 //ī�޶� ��ü�� �����͸� ���� ��� ����(����ŧ�� ���� ����� ���Ͽ� ī�޶��� ��ġ ���͸� �߰�)
cbuffer cbCameraInfo : register(b1)
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
    float3 gvCameraPosition : packoffset(c8);
};

 //���� ��ü�� �����͸� ���� ��� ����(���� ��ü�� ���� ���� ��ȣ�� �߰�)
cbuffer cbGameObjectInfo : register(b2)
{
    matrix gmtxGameObject : packoffset(c0);
    uint gnMaterial : packoffset(c4);
};

cbuffer cbMaterial : register(b3)
{
    MATERIAL gMaterials[MAX_MATERIALS];
};

cbuffer cbLights : register(b4)
{
    LIGHT gLights[MAX_LIGHTS];
    float4 gcGlobalAmbientLight;
};



struct VS_DIFFUSED_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_DIFFUSED_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_DIFFUSED_OUTPUT VSPlayer(VS_DIFFUSED_INPUT input)
{
    VS_DIFFUSED_OUTPUT output;
    output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxPlayerWorld),
gmtxView), gmtxProjection);
    output.color = input.color;
    return (output);
}

float4 PSPlayer(VS_DIFFUSED_OUTPUT input) : SV_TARGET
{
    return (input.color);
}

// ���� ������ ���
#define _WITH_VERTEX_LIGHTING

// ���� ���̴��� �Է� ���� ����
struct VS_LIGHTING_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

// ���� ���̴��� ��� ���� ����
struct VS_LIGHTING_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
#ifdef _WITH_VERTEX_LIGHTING
    float4 color : COLOR;
#else
 float3 normalW : NORMAL;
#endif
};



float4 DirectionalLight(int nIndex, float3 vNormal, float3 vToCamera)
{
    float3 vToLight = -gLights[nIndex].m_vDirection;
    float fDiffuseFactor = dot(vToLight, vNormal);
    float fSpecularFactor = 0.0f;
    if (fDiffuseFactor > 0.0f)
    {
        if (gMaterials[gnMaterial].m_cSpecular.a != 0.0f)
        {
#ifdef _WITH_REFLECT
 float3 vReflect = reflect(-vToLight, vNormal);
 fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), 
gMaterials[gnMaterial].m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
            float3 vHalf = normalize(vToCamera + vToLight);
#else
#endif
            // float3 vHalf = float3(0.0f, 1.0f, 0.0f);
            fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f),
gMaterials[gnMaterial].m_cSpecular.a);
#endif
        }
    }
    return ((gLights[nIndex].m_cAmbient * gMaterials[gnMaterial].m_cAmbient) +
(gLights[nIndex].m_cDiffuse * fDiffuseFactor * gMaterials[gnMaterial].m_cDiffuse) +
(gLights[nIndex].m_cSpecular * fSpecularFactor * gMaterials[gnMaterial].m_cSpecular));
}


float4 PointLight(int nIndex, float3 vPosition, float3 vNormal, float3 vToCamera)
{
    float3 vToLight = gLights[nIndex].m_vPosition - vPosition;
    float fDistance = length(vToLight);
    if (fDistance <= gLights[nIndex].m_fRange)
    {
        float fSpecularFactor = 0.0f;
        vToLight /= fDistance;
        float fDiffuseFactor = dot(vToLight, vNormal);
        if (fDiffuseFactor > 0.0f)
        {
            if (gMaterials[gnMaterial].m_cSpecular.a != 0.0f)
            {
#ifdef _WITH_REFLECT
 float3 vReflect = reflect(-vToLight, vNormal);
 fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), 
gMaterials[gnMaterial].m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
                float3 vHalf = normalize(vToCamera + vToLight);
#else
#endif
                // float3 vHalf = float3(0.0f, 1.0f, 0.0f);
                fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f),
gMaterials[gnMaterial].m_cSpecular.a);
#endif
            }
        }
        float fAttenuationFactor = 1.0f / dot(gLights[nIndex].m_vAttenuation, float3(1.0f,
fDistance, fDistance * fDistance));
        return (((gLights[nIndex].m_cAmbient * gMaterials[gnMaterial].m_cAmbient) +
(gLights[nIndex].m_cDiffuse * fDiffuseFactor * gMaterials[gnMaterial].m_cDiffuse) +
(gLights[nIndex].m_cSpecular * fSpecularFactor * gMaterials[gnMaterial].m_cSpecular)) *
fAttenuationFactor);
    }
    return (float4(0.0f, 0.0f, 0.0f, 0.0f));
}


float4 SpotLight(int nIndex, float3 vPosition, float3 vNormal, float3 vToCamera)
{
    float3 vToLight = gLights[nIndex].m_vPosition - vPosition;
    float fDistance = length(vToLight);
    if (fDistance <= gLights[nIndex].m_fRange)
    {
        float fSpecularFactor = 0.0f;
        vToLight /= fDistance;
        float fDiffuseFactor = dot(vToLight, vNormal);
        if (fDiffuseFactor > 0.0f)
        {
            if (gMaterials[gnMaterial].m_cSpecular.a != 0.0f)
            {
#ifdef _WITH_REFLECT
 float3 vReflect = reflect(-vToLight, vNormal);
 fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), 
gMaterials[gnMaterial].m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
                float3 vHalf = normalize(vToCamera + vToLight);
#else
#endif
                // float3 vHalf = float3(0.0f, 1.0f, 0.0f);
                fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f),
gMaterials[gnMaterial].m_cSpecular.a);
#endif
            }
        }
#ifdef _WITH_THETA_PHI_CONES
        float fAlpha = max(dot(-vToLight, gLights[nIndex].m_vDirection), 0.0f);
        float fSpotFactor = pow(max(((fAlpha - gLights[nIndex].m_fPhi) /
(gLights[nIndex].m_fTheta - gLights[nIndex].m_fPhi)), 0.0f), gLights[nIndex].m_fFalloff);
#else
 float fSpotFactor = pow(max(dot(-vToLight, gLights[i].m_vDirection), 0.0f), 
gLights[i].m_fFalloff);
#endif
        float fAttenuationFactor = 1.0f / dot(gLights[nIndex].m_vAttenuation, float3(1.0f,
fDistance, fDistance * fDistance));
        return (((gLights[nIndex].m_cAmbient * gMaterials[gnMaterial].m_cAmbient) +
(gLights[nIndex].m_cDiffuse * fDiffuseFactor * gMaterials[gnMaterial].m_cDiffuse) +
(gLights[nIndex].m_cSpecular * fSpecularFactor * gMaterials[gnMaterial].m_cSpecular)) *
fAttenuationFactor * fSpotFactor);
    }
    return (float4(0.0f, 0.0f, 0.0f, 0.0f));
}

float4 Lighting(float3 vPosition, float3 vNormal)
{
    float3 vCameraPosition = float3(gvCameraPosition.x, gvCameraPosition.y,
gvCameraPosition.z);
    float3 vToCamera = normalize(vCameraPosition - vPosition);
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        if (gLights[i].m_bEnable)
        {
            if (gLights[i].m_nType == DIRECTIONAL_LIGHT)
            {
                cColor += DirectionalLight(i, vNormal, vToCamera);
            }
            else if (gLights[i].m_nType == POINT_LIGHT)
            {
                cColor += PointLight(i, vPosition, vNormal, vToCamera);
            }
            else if (gLights[i].m_nType == SPOT_LIGHT)
            {
                cColor += SpotLight(i, vPosition, vNormal, vToCamera);
            }
        }
    }
    cColor += (gcGlobalAmbientLight * gMaterials[gnMaterial].m_cAmbient);
    cColor.a = gMaterials[gnMaterial].m_cDiffuse.a;
    return (cColor);
}

// ���� ���̴� �Լ�
VS_LIGHTING_OUTPUT VSLighting(VS_LIGHTING_INPUT input)
{
    VS_LIGHTING_OUTPUT output;
    output.positionW = (float3) mul(float4(input.position, 1.0f), gmtxGameObject);
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
    float3 normalW = mul(input.normal, (float3x3) gmtxGameObject);
#ifdef _WITH_VERTEX_LIGHTING
    output.color = Lighting(output.positionW, normalize(normalW));
#else
 output.normalW = normalW;
#endif
    return (output);
}

// �ȼ� ���̴� �Լ�
float4 PSLighting(VS_LIGHTING_OUTPUT input) : SV_TARGET
{
#ifdef _WITH_VERTEX_LIGHTING
    return (input.color);
#else
 float3 normalW = normalize(input.normalW);
 float4 color = Lighting(input.positionW, normalW);
 return(color);
#endif
}



/*
// ���� ��ü�� ������ ���� ��� ���۸� �����Ѵ�.
cbuffer cbGameObjectInfo : register(b0)
{
    matrix gmtxWorld : packoffset(c0);
};
 
// ī�޶��� ������ ���� ��� ���۸� �����Ѵ�.
cbuffer cbCameraInfo : register(b1)
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
};

// ���� ���̴��� �Է��� ���� ����ü�� �����Ѵ�.
struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

// ���� ���̴��� ���(�ȼ� ���̴��� �Է�)�� ���� ����ü�� �����Ѵ�.
struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

// ���� ���̴��� �����Ѵ�.
VS_OUTPUT VSDiffused(VS_INPUT input)
{
    VS_OUTPUT output;
    // ������ ��ȯ(���� ��ȯ, ī�޶� ��ȯ, ���� ��ȯ)�Ѵ�.
    output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView),
    gmtxProjection);
    output.color = input.color;
    return (output);
}

// �ȼ� ���̴��� �����Ѵ�.
float4 PSDiffused(VS_OUTPUT input) : SV_TARGET
{
    return (input.color);
}

// �ν��Ͻ� �����͸� ���� ����ü�̴�.
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
*/

// ----- <6-1> ���� ----- //

/*
// ���� ���̴��� �����Ѵ�. ���� Ÿ�� �簢�� ��ü�� �ﰢ�� 2���� ǥ���Ѵ�.
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

// �ȼ� ���̴��� ������ ���� �����Ͽ� ������ ���� ������ ���� ����� �� �� �ִ�.
/*
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    cColor.r = input.x /  FRAME_BUFFER_WIDTH;
    return (cColor);
}
*/

// �ȼ� ���̴��� ������ ���� �����Ͽ� ������ ���� ������ ���� ����� �� �� �ִ�.
/*
float4 PSMain(float4 input : SV_POSITION) : SV_TARGET
{
    float4 cColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    cColor.r = input.x / FRAME_BUFFER_WIDTH;
    cColor.g = input.y / FRAME_BUFFER_HEIGHT;
    return (cColor);
}
*/

// �ȼ� ���̴��� ������ ���� �����Ͽ� ������ ���� ������ ���� ����� �� �� �ִ�.
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

// �ȼ� ���̴��� ������ ���� �����Ͽ� ������ ���� ������ ���� ����� �� �� �ִ�.
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

// �ȼ� ���̴��� ������ ���� �����Ͽ� ������ ���� ������ ���� ����� �� �� �ִ�
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

// �ȼ� ���̴��� ������ ���� �����Ͽ� ������ ���� ������ ���� ����� �� �� �ִ�.
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

// �ȼ� ���̴��� ������ ���� �����Ͽ� ������ ���� ������ ���� ����� �� �� �ִ�.
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

// �ȼ� ���̴��� ������ ���� �����Ͽ� ������ ���� ������ ���� ����� �� �� �ִ�.
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

// �ȼ� ���̴��� ������ ���� �����Ͽ� ������ ���� ������ ���� ����� �� �� �ִ�.
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

// �ȼ� ���̴��� ������ ���� �����Ͽ� ������ ���� ������ ���� ����� �� �� �ִ�.
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

// �ȼ� ���̴��� ������ ���� �����Ͽ� ������ ���� ������ ���� ����� �� �� �ִ�.
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

// �ȼ� ���̴��� ������ ���� �����Ͽ� ������ ���� ������ ���� ����� �� �� �ִ�.
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
// �ȼ� ���̴��� ������ ���� �����Ͽ� ������ ���� ������ ���� ����� �� �� �ִ�.
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