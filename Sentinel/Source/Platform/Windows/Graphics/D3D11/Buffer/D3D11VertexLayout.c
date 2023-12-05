#include "stpch.h"
#include "Graphics/D3D11/Buffer/D3D11VertexLayout.h"
#include "Graphics/D3D11/Device/D3D11Device.h"
#include "Graphics/Material/Shader.h"

#define D3D11_NO_HELPERS
#define CINTERFACE
#define COBJMACROS

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma warning(push)
#pragma warning(disable : 6011)

static ID3D11InputLayout* inputLayout;

void Sentinel_D3D11VertexLayout_Init(ID3D10Blob* vertexShaderbytecode) {
    ID3D11ShaderReflection* pVertexShaderReflection = NULL;
    if (FAILED(D3DReflect(
            ID3D10Blob_GetBufferPointer(vertexShaderbytecode),
            ID3D10Blob_GetBufferSize(vertexShaderbytecode),
            &IID_ID3D11ShaderReflection,
            (void**)&pVertexShaderReflection))) {
        return;
    }

    D3D11_SHADER_DESC shaderDesc;
    pVertexShaderReflection->lpVtbl->GetDesc(pVertexShaderReflection, &shaderDesc);

    D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc;
    inputLayoutDesc = (D3D11_INPUT_ELEMENT_DESC*)Sentinel_Malloc(
        (ULLong)(sizeof(D3D11_INPUT_ELEMENT_DESC) * shaderDesc.InputParameters));

    for (UInt i = 0; i < shaderDesc.InputParameters; i++) {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
        pVertexShaderReflection->lpVtbl->GetInputParameterDesc(pVertexShaderReflection, i, &paramDesc);

        D3D11_INPUT_ELEMENT_DESC elementDesc;
        elementDesc.SemanticName = paramDesc.SemanticName;
        elementDesc.SemanticIndex = paramDesc.SemanticIndex;
        elementDesc.InputSlot = 0;
        elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        elementDesc.InstanceDataStepRate = 0;

        // determine DXGI format
        if (paramDesc.Mask == 1) {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                elementDesc.Format = DXGI_FORMAT_R32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                elementDesc.Format = DXGI_FORMAT_R32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
        } else if (paramDesc.Mask <= 3) {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
        } else if (paramDesc.Mask <= 7) {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        } else if (paramDesc.Mask <= 15) {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }

        // save element desc
        inputLayoutDesc[i] = elementDesc;
    }

    ID3D11Device_CreateInputLayout(
        Sentinel_D3D11Device_GetDevice(),
        inputLayoutDesc,
        shaderDesc.InputParameters,
        ID3D10Blob_GetBufferPointer(vertexShaderbytecode),
        ID3D10Blob_GetBufferSize(vertexShaderbytecode),
        &inputLayout);

    pVertexShaderReflection->lpVtbl->Release(pVertexShaderReflection);

    Sentinel_Free(inputLayoutDesc);
}

void Sentinel_D3D11VertexLayout_Deinit() {
    ID3D11InputLayout_Release(inputLayout);
}

void Sentinel_D3D11VertexLayout_Bind() {
    ID3D11DeviceContext_IASetPrimitiveTopology(
        Sentinel_D3D11Device_GetContext(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ID3D11DeviceContext_IASetInputLayout(Sentinel_D3D11Device_GetContext(), inputLayout);
}

#pragma warning(pop)
