#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"
#include "Sentinel/Graphics/Texture/DepthTexture2DData.h"

namespace Sentinel {
    struct SwapchainData;

    class DepthTexture2DAPI {
    public:
        static DepthTexture2DData* CreateDepthTexture2DData(
            PoolAllocator<DepthTexture2DData>& allocator,
            ContextData* context,
            const UInt16 width,
            const UInt16 height,
            const DepthFormat format,
            const Bool attachToSwapchain = false);

        static void Clear(DepthTexture2DData* dataObject);

        static void Clean(DepthTexture2DData* dataObject);

        static void Bind(DepthTexture2DData* dataObject, UInt8 slot, const ShaderType shaderType);

        static void Unbind(DepthTexture2DData* dataObject);

        static void Resize(DepthTexture2DData* dataObject, UInt16 width, UInt16 height);

        inline static void SetSwapchainTarget(DepthTexture2DData* dataObject, Bool value) {
            dataObject->m_SwapchainTarget = value;
        }

        inline static void* GetPixelData(DepthTexture2DData* dataObject) { return dataObject->m_PixelData; }

#if ST_RENDERER_DX11
        inline static ID3D11Texture2D* GetNativeTexture(DepthTexture2DData* dataObject) {
            return dataObject->m_NativeTex;
        }

        inline static ID3D11DepthStencilView* GetNativeDSV(DepthTexture2DData* dataObject) {
            return dataObject->m_NativeDSV;
        }

        inline static ID3D11ShaderResourceView* GetNativeSRV(DepthTexture2DData* dataObject) {
            return dataObject->m_NativeSRV;
        }
#endif  // ST_RENDERER_DX11

    private:
        static void Create(DepthTexture2DData* dataObject);
    };
}  // namespace Sentinel