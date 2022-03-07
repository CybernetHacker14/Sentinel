#type vertex
#pragma pack_matrix(row_major)
cbuffer Camera : register(b0) { float4x4 u_ViewProjection; }

struct VOut {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

VOut VShader(float4 position : POSITION, float2 texCoord : TEXCOORD) {
	VOut output;

	output.position = mul(position, u_ViewProjection);
	output.texCoord = texCoord;

	return output;
}

#type pixel
struct VOut {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

Texture2D testTexture;
SamplerState textureSampler : register(s0);

float4 PShader(VOut input) : SV_TARGET
{
	return testTexture.Sample(textureSampler, input.texCoord);
}
