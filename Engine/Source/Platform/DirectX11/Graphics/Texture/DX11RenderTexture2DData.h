#pragma once

#include "Sentinel/Graphics/Texture/RenderTexture2DData.h"
#include "Platform/DirectX11/Graphics/Device/DX11SwapchainData.h"

struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;

namespace Sentinel {
    struct DX11RenderTexture2DData final: public RenderTexture2DData {
    private:
        ID3D11Texture2D* m_NativeTexture;
        ID3D11RenderTargetView* m_NativeRTV;
        ID3D11ShaderResourceView* m_NativeSRV;
        ID3D11UnorderedAccessView* m_NativeUAV;

        DX11SwapchainData* m_TargetSwapchain;

    private:
        friend class DX11RenderTexture2DAPI;
        friend class DX11FramebufferAPI;
        friend class DX11SwapchainAPI;
    };
}  // namespace Sentinel