#pragma once

#include "Sentinel/Common/Common.h"

#ifdef ST_RENDERER_DX11
struct ID3D11InputLayout;
#endif  // ST_RENDERER_DX11

namespace Sentinel {
    struct ContextData;

    struct VertexbufferLayoutData {
    public:
        ContextData* Context;

    private:
        UInt32 m_Stride;

    private:
#ifdef ST_RENDERER_DX11
        ID3D11InputLayout* m_Layout;
#endif  // ST_RENDERER_DX11

        friend class VertexbufferLayoutAPI;
    };
}  // namespace Sentinel