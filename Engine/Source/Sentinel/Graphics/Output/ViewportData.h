#pragma once

#include "Sentinel/Common/Common.h"

#if ST_RENDERER_DX11
    #include <d3d11.h>
#endif  // ST_RENDERER_DX11

namespace Sentinel {
    struct ContextData;

    struct ViewportData {
    public:
        ContextData* Context;

        // Since viewport will be probably accessed by many classes, even outside the rendering domain, keeping
        // fields public.
    public:
        Float x, y;
        Float width, height;
        Float depthMin, depthMax;

    private:
#if ST_RENDERER_DX11
        D3D11_VIEWPORT m_Viewport;
#endif  // ST_RENDERER_DX11

        friend class ViewportAPI;
    };
}  // namespace Sentinel