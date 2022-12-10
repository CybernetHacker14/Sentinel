#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Memory/PoolAllocator.h"
#include "Sentinel/Graphics/Texture/RenderTexture2DData.h"

#include <glm/fwd.hpp>

namespace Sentinel {
    class RenderTexture2DAPI {
    public:
        static RenderTexture2DData* CreateRenderTexture2DData(
            PoolAllocator<RenderTexture2DData>& allocator,
            ContextData* context,
            const UInt16 width,
            const UInt16 height,
            const ColorFormat format);

        // Used to create an RT which will be attached to swapchain
        static RenderTexture2DData* CreateRenderTexture2DData(
            PoolAllocator<RenderTexture2DData>& allocator, ContextData* context, SwapchainData* swapchain);

        static void Clear(RenderTexture2DData* dataObject, const glm::vec4& clearColor);

        static void Clean(RenderTexture2DData* dataObject);

        static void Bind(RenderTexture2DData* dataObject, UInt8 slot, const ShaderType shaderType);

        static void Unbind(RenderTexture2DData* dataObject);

        static void Resize(RenderTexture2DData* dataObject, UInt16 width, UInt16 height);

        inline static void SetSwapchainTarget(RenderTexture2DData* dataObject, Bool value) {
            dataObject->m_SwapchainTarget = value;
        }

        inline static void* GetPixelData(RenderTexture2DData* dataObject) { return dataObject->m_PixelData; }

#ifdef ST_RENDERER_DX11
        inline static ID3D11Texture2D* GetNativeTexture(RenderTexture2DData* dataObject) {
            return dataObject->m_NativeTexture;
        }

        inline static ID3D11RenderTargetView* GetNativeRTV(RenderTexture2DData* dataObject) {
            return dataObject->m_NativeRTV;
        }

        inline static ID3D11ShaderResourceView* GetNativeSRV(RenderTexture2DData* dataObject) {
            return dataObject->m_NativeSRV;
        }

        inline static ID3D11UnorderedAccessView* GetNativeUAV(RenderTexture2DData* dataObject) {
            return dataObject->m_NativeUAV;
        }
#endif  // ST_RENDERER_DX11

    private:
        static void Create(RenderTexture2DData* dataObject);
        static void Create(RenderTexture2DData* dataObject, SwapchainData* swapchain);
    };
}  // namespace Sentinel