#pragma once

#include "Sentinel/Graphics/Texture/DepthTexture2DData.h"

struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;

namespace Sentinel {
    struct DX11DepthTexture2DData final: public DepthTexture2DData {
    private:
        ID3D11Texture2D* m_NativeTexture;
        ID3D11DepthStencilView* m_NativeDSV = nullptr;
        ID3D11ShaderResourceView* m_NativeSRV;

    private:
        friend class DX11DepthTexture2DAPI;
        friend class DX11FramebufferAPI;
        friend class DX11SwapchainAPI;
    };
}  // namespace Sentinel