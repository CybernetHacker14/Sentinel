struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 col : COLOR0;
    float2 uv : TEXCOORD;
};

Texture2D testTexture;
SamplerState texSampler : register(s0);

float4 ps(PS_INPUT input) : SV_Target
{
    return float4(input.col, 1.f) * testTexture.Sample(texSampler, input.uv);
}
