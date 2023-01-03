#type vertex
#pragma pack_matrix(row_major)
cbuffer Camera: register(b0) {
    float4x4 u_ViewProjection;
}

struct VOut {
    float4 position: SV_POSITION;
    float2 texCoord: TEXCOORD;
};

VOut VShader(float4 position : POSITION, float2 texCoord : TEXCOORD) {
    VOut output;

    output.position = mul(position, u_ViewProjection);
    output.texCoord = texCoord;

    return output;
}

#type pixel
struct VOut {
    float4 position: SV_POSITION;
    float2 texCoord: TEXCOORD;
};

float4 PShader(VOut input) : SV_TARGET {
    return float4(0.1f, 0.5f, 0.1f, 1.0f);
}
