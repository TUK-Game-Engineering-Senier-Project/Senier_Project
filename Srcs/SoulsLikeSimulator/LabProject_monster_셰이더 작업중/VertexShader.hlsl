struct VSInput
{
    float3 Position : POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD;
};

struct VSOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD;
};

cbuffer MatrixBuffer : register(b0)
{
    float4x4 gWorldViewProj; // World-View-Projection matrix
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;
    output.Position = mul(float4(input.Position, 1.0), gWorldViewProj); // Apply world-view-projection matrix
    output.Color = input.Color;
    output.TexCoord = input.TexCoord;
    return output;
}