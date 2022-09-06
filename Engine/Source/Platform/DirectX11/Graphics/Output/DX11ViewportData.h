#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Output/ViewportData.h"

#include <d3d11.h>

namespace Sentinel {
    struct DX11ViewportData final: public ViewportData {
    private:
        D3D11_VIEWPORT m_Viewport;

    private:
        friend class DX11ViewportAPI;
    };
}  // namespace Sentinel