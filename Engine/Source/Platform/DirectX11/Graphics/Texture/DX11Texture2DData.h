#pragma once

#include "Sentinel/Graphics/Texture/Texture2DData.h"

struct ID3D11Texture2D;
struct ID3D11SamplerState;
struct ID3D11ShaderResourceView;

namespace Sentinel {
    struct DX11Texture2DData final: public Texture2DData {
    private:
        ID3D11Texture2D* m_Texture2D;
        ID3D11SamplerState* m_SamplerState;
        ID3D11ShaderResourceView* m_ResourceView;

    private:
        friend class DX11Texture2DAPI;
    };
}  // namespace Sentinel