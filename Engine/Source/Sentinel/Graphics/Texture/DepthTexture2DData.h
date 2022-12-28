#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Material/ShaderData.h"

#if ST_RENDERER_DX11
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
#endif  // ST_RENDERER_DX11

namespace Sentinel {
    enum class DepthFormat : UInt8 { NONE = 0, D24S8UINT = 45 };
    struct ContextData;
    struct SwapchainData;

    struct DepthTexture2DData {
    public:
        ContextData* Context;
        SwapchainData* TargetSwapchain = nullptr;

    protected:
        void* m_PixelData;
        UInt16 m_Width, m_Height;
        UInt8 m_BindSlot;
        DepthFormat m_Format = DepthFormat::NONE;
        ShaderType m_BindType = ShaderType::NONE;
        Bool m_SwapchainTarget = false;

    private:
#if ST_RENDERER_DX11
        ID3D11Texture2D* m_NativeTex;
        ID3D11DepthStencilView* m_NativeDSV;
        ID3D11ShaderResourceView* m_NativeSRV;
#endif  // ST_RENDERER_DX11

        friend class DepthTexture2DAPI;
    };
}  // namespace Sentinel