#pragma once

#include "Sentinel/Graphics/Texture/RenderTexture2DData.h"

struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;

namespace Sentinel {
    struct DX11RenderTexture2DData final: public RenderTexture2DData {
    private:
        ID3D11Texture2D* m_NativeColorTexture;
        ID3D11RenderTargetView* m_NativeColorRTV;
        ID3D11ShaderResourceView* m_NativeColorSRV;
        ID3D11UnorderedAccessView* m_NativeColorUAV;

        ID3D11Texture2D* m_NativeDepthTexture;
        ID3D11DepthStencilView* m_NativeDepthDSV;
        ID3D11ShaderResourceView* m_NativeDepthSRV;

    private:
        friend class DX11RenderTexture2DAPI;
    };
}  // namespace Sentinel