#include "stpch.h"

#ifdef ST_RENDERER_DX11
    #include "Sentinel/Graphics/Buffer/VertexbufferLayoutAPI.h"
    #include "Sentinel/Graphics/Material/ShaderAPI.h"
    #include "Sentinel/Graphics/Device/ContextAPI.h"

    #include "Platform/Windows/DirectX11/Graphics/Core/DX11Common.h"

    #include "Sentinel/Common/Containers/Vector.h"
    #include "Sentinel/Common/Core/Assert.h"

    #include <sparse_map.h>

namespace Sentinel {
    static tsl::sparse_map<DXGI_FORMAT, UInt32> s_ShaderDataTypeSizeMap = {
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

    VertexbufferLayoutData* VertexbufferLayoutAPI::CreateVertexbufferLayoutData(
        PoolAllocator<VertexbufferLayoutData>& allocator, ContextData* context) {
        VertexbufferLayoutData* layoutObject = allocator.New();
        layoutObject->Context = context;
        return layoutObject;
    }

    void VertexbufferLayoutAPI::CreateLayout(VertexbufferLayoutData* dataObject, ShaderData* shader) {
        ID3D10Blob* binary = ShaderAPI::GetBinary(shader, ShaderType::VERTEX);

        // Excerpts from https://gist.github.com/mobius/b678970c61a93c81fffef1936734909f

        ID3D11ShaderReflection* vertexShaderReflection = nullptr;
        if (FAILED(D3DReflect(
                binary->GetBufferPointer(),
                binary->GetBufferSize(),
                IID_ID3D11ShaderReflection,
                (void**)&vertexShaderReflection))) {
            ST_BREAKPOINT_ASSERT(false, "Cannot reflect DirectX11 Vertex Shader!")
        }

        D3D11_SHADER_DESC shaderDescription;
        vertexShaderReflection->GetDesc(&shaderDescription);

        dataObject->m_Stride = 0;

        Vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDescriptions;
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

            inputLayoutDescriptions.Emplace_Back(elementDescription);

            dataObject->m_Stride += s_ShaderDataTypeSizeMap.at(elementDescription.Format);
        }

        ContextAPI::GetDevice(dataObject->Context)
            ->CreateInputLayout(
                &inputLayoutDescriptions[0],
                static_cast<UInt32>(inputLayoutDescriptions.Size()),
                binary->GetBufferPointer(),
                binary->GetBufferSize(),
                &(dataObject->m_Layout));

        vertexShaderReflection->Release();
    }

    void VertexbufferLayoutAPI::Bind(VertexbufferLayoutData* dataObject) {
        ContextAPI::GetNativeContext(dataObject->Context)
            ->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        ContextAPI::GetNativeContext(dataObject->Context)->IASetInputLayout(dataObject->m_Layout);
    }

    void VertexbufferLayoutAPI::Unbind(VertexbufferLayoutData* dataObject) {
        ContextAPI::GetNativeContext(dataObject->Context)->IASetInputLayout(nullptr);
    }

    void VertexbufferLayoutAPI::Clean(VertexbufferLayoutData* dataObject) {
        dataObject->m_Layout->Release();
        dataObject->m_Layout = 0;
    }
}  // namespace Sentinel
#endif  // ST_RENDERER_DX11