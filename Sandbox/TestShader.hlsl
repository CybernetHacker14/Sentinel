#type vertex
struct VOut
{
    float4 position : SV_POSITION;
};

VOut VShader(float4 position : POSITION)
{
    VOut output;

    output.position = position;

    return output;
}

#type pixel
float4 PShader(float4 position : SV_POSITION) : SV_TARGET
{
    return float4(1,1,1,1);
}
