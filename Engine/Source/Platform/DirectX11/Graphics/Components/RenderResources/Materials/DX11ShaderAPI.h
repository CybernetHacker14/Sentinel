#pragma once

#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Components/RenderResources/Materials/ShaderAPI.h"

namespace Sentinel {
    class DX11ShaderAPI final: public ShaderAPI {
    public:
        static class _init {
        public:
            inline _init() {
                if (Backend::GetAPI() == Backend::API::DirectX11) {
                    m_BindFunction = DX11ShaderAPI::Bind;
                    m_ReloadFunction = DX11ShaderAPI::Reload;
                    m_CleanFunction = DX11ShaderAPI::Clean;
                }
            }
        } _initializer;

    public:
        static ID3DBlob* GetVertexShaderBinary(DX11ShaderData* dataObject) {
            return dataObject->m_BinaryMap.at(ShaderType::VERTEX);
        }

        static ID3DBlob* GetPixelShaderBinary(DX11ShaderData* dataObject) {
            return dataObject->m_BinaryMap.at(ShaderType::PIXEL);
        }

        static ID3DBlob* GetComputeShaderBinary(DX11ShaderData* dataObject) {
            return dataObject->m_BinaryMap.at(ShaderType::COMPUTE);
        }

    private:
        static void Bind(ShaderData* dataObject);
        static void Reload(ShaderData* dataObject);
        static void Clean(ShaderData* dataObject);

        static STL::unordered_map<ShaderType, STL::string> PreprocessSource(const STL::string& source);

        static void CompileFromSource(DX11ShaderData* dataObject);
        static void Load(DX11ShaderData* dataObject);
        static void Reset(DX11ShaderData* dataObject);

    private:
        friend class ShaderAPI;
    };
}  // namespace Sentinel