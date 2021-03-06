#pragma once

#include "Sentinel/Graphics/Definitions/Backend.h"
#include "Sentinel/Graphics/Components/RenderResources/Materials/Texture2DAPI.h"

namespace Sentinel {
    class DX11Texture2DAPI final: public Texture2DAPI {
    public:
        static class _init {
        public:
            inline _init() {
                if (Backend::GetAPI() == Backend::API::DirectX11) {
                    m_BindFunction = DX11Texture2DAPI::Bind;
                    m_UnbindFunction = DX11Texture2DAPI::Unbind;
                    m_CleanFunction = DX11Texture2DAPI::Clean;
                }
            }
        } _initializer;

    private:
        static void Bind(Texture2DData* dataObject, UInt32 slot, const ShaderType shaderType);
        static void Unbind(Texture2DData* dataObject, UInt32 slot, const ShaderType shaderType);
        static void Clean(Texture2DData* dataObject);

        static void Load(DX11Texture2DData* dataObject);

    private:
        friend class Texture2DAPI;
        friend class _init;
    };

}  // namespace Sentinel