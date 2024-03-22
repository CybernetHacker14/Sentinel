cbuffer Camera : register(b0)
{
    float4x4 u_ViewProjection;
}

cbuffer ObjectColor : register(b1)
{
    float4 u_ObjectColor;
}

struct VS_INPUT
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 col : COLOR0;
    float2 uv : TEXCOORD;
};

PS_INPUT vs(VS_INPUT input)
{
    PS_INPUT output;
    output.pos = mul(float4(input.pos, 1.0f), u_ViewProjection);
    output.col = u_ObjectColor.xyz;
    output.uv = input.uv;
    return output;
}
