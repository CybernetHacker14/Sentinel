#pragma once

#include "Sentinel/Graphics/Common/Backend.h"
#include "Sentinel/Graphics/Texture/DepthTexture2DAPI.h"

#include "Platform/DirectX11/Graphics/Texture/DX11DepthTexture2DData.h"

namespace Sentinel {
    class DX11DepthTexture2DAPI final: public DepthTexture2DAPI {
    public:
        static class _init {
        public:
            inline _init() {
                if (Backend::GetAPI() == Backend::API::DirectX11) {
                    m_ClearFunction = DX11DepthTexture2DAPI::Clear;
                    m_CleanFunction = DX11DepthTexture2DAPI::Clean;
                    m_BindFunction = DX11DepthTexture2DAPI::Bind;
                    m_UnbindFunction = DX11DepthTexture2DAPI::Unbind;
                    m_ResizeFunction = DX11DepthTexture2DAPI::Resize;
                }
            }
        } _initializer;

    public:
        inline static ID3D11Texture2D* GetNativeTexture(DX11DepthTexture2DData* dataObject) {
            return dataObject->m_NativeTexture;
        }

        inline static ID3D11DepthStencilView* GetNativeRTV(DX11DepthTexture2DData* dataObject) {
            return dataObject->m_NativeDSV;
        }

        inline static ID3D11ShaderResourceView* GetNativeSRV(DX11DepthTexture2DData* dataObject) {
            return dataObject->m_NativeSRV;
        }

    private:
        static void Create(DX11DepthTexture2DData* dataObject);

    private:
        static void Clear(DepthTexture2DData* dataObject);
        static void Clean(DepthTexture2DData* dataObject);

        static void Resize(DepthTexture2DData* dataObject, UInt32 width, UInt32 height);

        static void Bind(DepthTexture2DData* dataObject, UInt32 slot, const ShaderType shaderType);
        static void Unbind(DepthTexture2DData* dataObject);

    private:
        friend class DepthTexture2DAPI;
    };
}  // namespace Sentinel