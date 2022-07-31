#include "stpch.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"
#include "Platform/DirectX11/Graphics/Buffer/DX11VertexbufferLayoutAPI.h"
#include "Platform/DirectX11/Graphics/Buffer/DX11VertexbufferLayoutData.h"

#include "Platform/DirectX11/Graphics/Material/DX11ShaderAPI.h"

#include "Platform/DirectX11/Graphics/Device/DX11ContextAPI.h"

namespace Sentinel {
    static STL::unordered_map<DXGI_FORMAT, UInt32> s_ShaderDataTypeSizeMap = {
        {DXGI_FORMAT_R32_FLOAT, 4},
        {DXGI_FORMAT_R32_UINT, 4},
        {DXGI_FORMAT_R32_SINT, 4},
        {DXGI_FORMAT_R32G32_FLOAT, 8},
        {DXGI_FORMAT_R32G32_UINT, 8},
        {DXGI_FORMAT_R32G32_SINT, 8},
        {DXGI_FORMAT_R32G32B32_FLOAT, 12},
        {DXGI_FORMAT_R32G32B32_UINT, 12},
        {DXGI_FORMAT_R32G32B32_SINT, 12},
        {DXGI_FORMAT_R32G32B32A32_FLOAT, 16},
        {DXGI_FORMAT_R32G32B32A32_UINT, 16},
        {DXGI_FORMAT_R32G32B32A32_SINT, 16}};

    DX11VertexbufferLayoutAPI::_init DX11VertexbufferLayoutAPI::_initializer;

    void DX11VertexbufferLayoutAPI::CreateLayout(VertexbufferLayoutData* dataObject, ShaderData* shader) {
        DX11VertexbufferLayoutData* layout = VertexbufferLayoutAPI::Cast<DX11VertexbufferLayoutData>(dataObject);
        ID3DBlob* binary = DX11ShaderAPI::GetVertexShaderBinary(ShaderAPI::Cast<DX11ShaderData>(shader));

        // Excerpts from https://gist.github.com/mobius/b678970c61a93c81fffef1936734909f

        ID3D11ShaderReflection* vertexShaderReflection = nullptr;
        if (FAILED(D3DReflect(
                binary->GetBufferPointer(),
                binary->GetBufferSize(),
                IID_ID3D11ShaderReflection,
                (void**)&vertexShaderReflection))) {
            ST_ENGINE_ASSERT(false, "Cannot reflect DirectX11 Vertex Shader!")
        }

        D3D11_SHADER_DESC shaderDescription;
        vertexShaderReflection->GetDesc(&shaderDescription);

        layout->m_Stride = 0;

        STL::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDescriptions;
        for (UInt32 i = 0; i < shaderDescription.InputParameters; i++) {
            D3D11_SIGNATURE_PARAMETER_DESC paramDescription;
            vertexShaderReflection->GetInputParameterDesc(i, &paramDescription);

            D3D11_INPUT_ELEMENT_DESC elementDescription;
            elementDescription.SemanticName = paramDescription.SemanticName;
            elementDescription.SemanticIndex = paramDescription.SemanticIndex;
            elementDescription.InputSlot = 0;
            elementDescription.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
            elementDescription.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            elementDescription.InstanceDataStepRate = 0;

            if (paramDescription.Mask == 1) {
                if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                    elementDescription.Format = DXGI_FORMAT_R32_FLOAT;
                else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                    elementDescription.Format = DXGI_FORMAT_R32_UINT;
                else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                    elementDescription.Format = DXGI_FORMAT_R32_SINT;
            } else if (paramDescription.Mask <= 3) {
                if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                    elementDescription.Format = DXGI_FORMAT_R32G32_FLOAT;
                else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                    elementDescription.Format = DXGI_FORMAT_R32G32_UINT;
                else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                    elementDescription.Format = DXGI_FORMAT_R32G32_SINT;
            } else if (paramDescription.Mask <= 7) {
                if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                    elementDescription.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                    elementDescription.Format = DXGI_FORMAT_R32G32B32_UINT;
                else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                    elementDescription.Format = DXGI_FORMAT_R32G32B32_SINT;
            } else if (paramDescription.Mask <= 15) {
                if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                    elementDescription.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                    elementDescription.Format = DXGI_FORMAT_R32G32B32A32_UINT;
                else if (paramDescription.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                    elementDescription.Format = DXGI_FORMAT_R32G32B32A32_SINT;
            }

            inputLayoutDescriptions.emplace_back(elementDescription);

            layout->m_Stride += s_ShaderDataTypeSizeMap.at(elementDescription.Format);
        }

        DX11ContextData* dContext = ContextAPI::Cast<DX11ContextData>(dataObject->Context);

        DX11ContextAPI::GetDevice(dContext)->CreateInputLayout(
            &inputLayoutDescriptions[0],
            static_cast<UInt32>(inputLayoutDescriptions.size()),
            binary->GetBufferPointer(),
            binary->GetBufferSize(),
            &layout->m_InputLayout);

        vertexShaderReflection->Release();
    }

    void DX11VertexbufferLayoutAPI::Bind(VertexbufferLayoutData* dataObject) {
        DX11VertexbufferLayoutData* layout = VertexbufferLayoutAPI::Cast<DX11VertexbufferLayoutData>(dataObject);
        DX11ContextData* dContext = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        DX11ContextAPI::GetNativeContext(dContext)->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        DX11ContextAPI::GetNativeContext(dContext)->IASetInputLayout(layout->m_InputLayout);
    }

    void DX11VertexbufferLayoutAPI::Unbind(VertexbufferLayoutData* dataObject) {
        DX11ContextData* dContext = ContextAPI::Cast<DX11ContextData>(dataObject->Context);
        DX11ContextAPI::GetNativeContext(dContext)->IASetInputLayout(nullptr);
    }

    void DX11VertexbufferLayoutAPI::Clean(VertexbufferLayoutData* dataObject) {
        DX11VertexbufferLayoutData* layout = VertexbufferLayoutAPI::Cast<DX11VertexbufferLayoutData>(dataObject);
        if (layout->m_InputLayout) layout->m_InputLayout->Release();
    }
}  // namespace Sentinel