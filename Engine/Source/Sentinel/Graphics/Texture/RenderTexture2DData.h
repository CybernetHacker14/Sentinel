#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Material/ShaderData.h"

#ifdef ST_RENDERER_DX11
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;
#endif  // ST_RENDERER_DX11

namespace Sentinel {
    enum class ColorFormat : UInt8 { NONE = 0, RGBA32F = 2, RGBA8UNORM = 28, R32SINT = 43 };
    struct ContextData;
    struct SwapchainData;

    struct RenderTexture2DData {
    public:
        ContextData* Context;
        SwapchainData* TargetSwapchain = nullptr;

    private:
        void* m_PixelData;
        UInt32 m_Width, m_Height;
        UInt32 m_BindSlot;
        ColorFormat m_Format = ColorFormat::NONE;
        ShaderType m_BindType = ShaderType::NONE;
        Bool m_SwapchainTarget = false;

    private:
#ifdef ST_RENDERER_DX11
        ID3D11Texture2D* m_NativeTexture;
        ID3D11RenderTargetView* m_NativeRTV;
        ID3D11ShaderResourceView* m_NativeSRV;
        ID3D11UnorderedAccessView* m_NativeUAV;
#endif  // ST_RENDERER_DX11

        friend class RenderTexture2DAPI;
    };
}  // namespace Sentinel