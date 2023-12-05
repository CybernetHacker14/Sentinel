#include "stpch.h"
#include "_EXPORT/Graphics/Material/ShaderDATA_EXPORT.h"
#include "Graphics/D3D11/Buffer/D3D11VertexLayout.h"
#include "Graphics/D3D11/Device/D3D11Device.h"
#include "Graphics/D3D11/Material/D3D11Shader.h"
#include "Graphics/Material/Shader.h"

#define D3D11_NO_HELPERS
#define CINTERFACE
#define COBJMACROS

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma warning(push)
#pragma warning(disable : 6011)

#pragma comment(lib, "d3dcompiler.lib")

static CChar entrypoint[][3] = {"vs", "ps", "cs"};
static CChar target[][7] = {"vs_5_0", "ps_5_0", "cs_5_0"};

static void Sentinel_D3D11Shader_CreateVertexShader(ShaderData* shader, const void* bytecode, ULLong bytecodeLength) {
    ID3D11VertexShader* vShader = NULL;
    ID3D11Device_CreateVertexShader(Sentinel_D3D11Device_GetDevice(), bytecode, bytecodeLength, NULL, &vShader);
    shader->native = vShader;
}

static void Sentinel_D3D11Shader_CreatePixelShader(ShaderData* shader, const void* bytecode, ULLong bytecodeLength) {
    ID3D11PixelShader* pShader = NULL;
    ID3D11Device_CreatePixelShader(Sentinel_D3D11Device_GetDevice(), bytecode, bytecodeLength, NULL, &pShader);
    shader->native = pShader;
}

static void Sentinel_D3D11Shader_CreateComputeShader(ShaderData* shader, const void* bytecode, ULLong bytcodeLength) {
    ID3D11ComputeShader* cShader = NULL;
    ID3D11Device_CreateComputeShader(Sentinel_D3D11Device_GetDevice(), bytecode, bytcodeLength, NULL, &cShader);
    shader->native = cShader;
}

static void Sentinel_D3D11Shader_BindVertexShader(ShaderData* shader) {
    ID3D11VertexShader* vShader = (ID3D11VertexShader*)(shader->native);
    ID3D11DeviceContext_VSSetShader(Sentinel_D3D11Device_GetContext(), vShader, NULL, 0);
}

static void Sentinel_D3D11Shader_BindPixelShader(ShaderData* shader) {
    ID3D11PixelShader* pShader = (ID3D11PixelShader*)(shader->native);
    ID3D11DeviceContext_PSSetShader(Sentinel_D3D11Device_GetContext(), pShader, NULL, 0);
}

static void Sentinel_D3D11Shader_BindComputeShader(ShaderData* shader) {
    ID3D11ComputeShader* cShader = (ID3D11ComputeShader*)(shader->native);
    ID3D11DeviceContext_CSSetShader(Sentinel_D3D11Device_GetContext(), cShader, NULL, 0);
}

static void Sentinel_D3D11Shader_DestroyVertexShader(ShaderData* shader) {
    ID3D11VertexShader* vShader = (ID3D11VertexShader*)(shader->native);
    ID3D11VertexShader_Release(vShader);
    shader->native = NULL;
}

static void Sentinel_D3D11Shader_DestroyPixelShader(ShaderData* shader) {
    ID3D11PixelShader* pShader = (ID3D11PixelShader*)(shader->native);
    ID3D11PixelShader_Release(pShader);
    shader->native = NULL;
}

static void Sentinel_D3D11Shader_DestroyComputeShader(ShaderData* shader) {
    ID3D11ComputeShader* cShader = (ID3D11ComputeShader*)(shader->native);
    ID3D11ComputeShader_Release(cShader);
    shader->native = NULL;
}

// Instead of bloating the main create function code with unnecessary if-else for different shader types,
// you can isolate and put standalone functions for each shader type in this array, according to the
// ShaderType's decomposed enum-int value
static void (*shaderCreatePFn[])(ShaderData*, const void*, ULLong) = {
    Sentinel_D3D11Shader_CreateVertexShader,
    Sentinel_D3D11Shader_CreatePixelShader,
    Sentinel_D3D11Shader_CreateComputeShader};

// Instead of bloating the main create function code with unnecessary if-else for different shader types,
// you can isolate and put standalone functions for each shader type in this array, according to the
// ShaderType's decomposed enum-int value
static void (*shaderBindPFn[])(ShaderData*) = {
    Sentinel_D3D11Shader_BindVertexShader,
    Sentinel_D3D11Shader_BindPixelShader,
    Sentinel_D3D11Shader_BindComputeShader};

// Instead of bloating the main destroy function code with unnecessary if-else for different shader types,
// you can isolate and put standalone functions for each shader type in this array, according to the
// ShaderType's decomposed enum-int value
static void (*shaderDestroyPFn[])(ShaderData*) = {
    Sentinel_D3D11Shader_DestroyVertexShader,
    Sentinel_D3D11Shader_DestroyPixelShader,
    Sentinel_D3D11Shader_DestroyComputeShader};

void Sentinel_D3D11Shader_Create(ShaderData* shader, CChar* source, ULLong length) {
    UINT shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS;

#ifdef ST_DEBUG
    shaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL0 | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#else
    shaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif  // ST_DEBUG

    ID3DBlob* code = NULL;
    const void* byteCode;
    ULLong byteCodeSize;
    ID3DBlob* error;
    D3DCompile(
        source,
        length,
        NULL,
        NULL,
        NULL,
        entrypoint[shader->type],
        target[shader->type],
        shaderFlags,
        0,
        &code,
        &error);
    byteCode = ID3D10Blob_GetBufferPointer(code);
    byteCodeSize = ID3D10Blob_GetBufferSize(code);

    shaderCreatePFn[shader->type](shader, byteCode, byteCodeSize);

    if (shader->type == VERTEX) Sentinel_D3D11VertexLayout_Init(code);

    ID3D10Blob_Release(code);
}

void Sentinel_D3D11Shader_Destroy(ShaderData* shader) {
    shaderDestroyPFn[shader->type](shader);
}

void Sentinel_D3D11Shader_Bind(ShaderData* shader) {
    shaderBindPFn[shader->type](shader);
}

#pragma warning(pop)
