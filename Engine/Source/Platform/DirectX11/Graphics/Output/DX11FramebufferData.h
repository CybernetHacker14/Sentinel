#pragma once

#include "Sentinel/Graphics/Output/FramebufferData.h"

#include <d3d11.h>

namespace Sentinel {
    struct DX11FramebufferData final: public FramebufferData {
    private:
        D3D11_VIEWPORT m_Viewport;

    private:
        friend class DX11FramebufferAPI;
    };
}  // namespace Sentinel