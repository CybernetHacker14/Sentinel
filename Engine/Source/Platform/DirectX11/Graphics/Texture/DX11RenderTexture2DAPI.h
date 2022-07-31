#pragma once

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Texture/RenderTexture2DAPI.h"

#include "Platform/DirectX11/Graphics/Texture/DX11RenderTexture2DData.h"

namespace Sentinel {
    class DX11RenderTexture2DAPI final: public RenderTexture2DAPI {
    public:
        static class _init {
        public:
            inline _init() {
                if (Backend::GetAPI() == Backend::API::DirectX11) {
                    m_ClearFunction = DX11RenderTexture2DAPI::Clear;
                    m_CleanFunction = DX11RenderTexture2DAPI::Clean;
                    m_ResizeFunction = DX11RenderTexture2DAPI::Resize;
                    m_BindFunction = DX11RenderTexture2DAPI::Bind;
                    m_UnbindFunction = DX11RenderTexture2DAPI::Unbind;
                }
            }
        } _initializer;

    public:
        inline static ID3D11Texture2D* GetNativeTexture(DX11RenderTexture2DData* dataObject) {
            return dataObject->m_NativeTexture;
        }

        inline static ID3D11RenderTargetView* GetNativeRTV(DX11RenderTexture2DData* dataObject) {
            return dataObject->m_NativeRTV;
        }

        inline static ID3D11ShaderResourceView* GetNativeSRV(DX11RenderTexture2DData* dataObject) {
            return dataObject->m_NativeSRV;
        }

        inline static ID3D11UnorderedAccessView* GetNativeUAV(DX11RenderTexture2DData* dataObject) {
            return dataObject->m_NativeUAV;
        }

    private:
        static void Create(DX11RenderTexture2DData* dataObject);

    private:
        static void Clear(RenderTexture2DData* dataObject, const glm::vec4& clearColor);
        static void Clean(RenderTexture2DData* dataObject);
        static void Resize(RenderTexture2DData* dataObject, UInt32 width, UInt32 height);

        static void Bind(RenderTexture2DData* dataObject, UInt32 slot, const ShaderType shaderType);
        static void Unbind(RenderTexture2DData* dataObject);

    private:
        friend class RenderTexture2DAPI;
    };
}  // namespace Sentinel