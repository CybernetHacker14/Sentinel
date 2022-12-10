#pragma once

#include "Sentinel/Common/Common.h"

#if ST_RENDERER_DX11
struct ID3D11Buffer;
#endif  // ST_RENDERER_DX11

namespace Sentinel {
    struct ContextData;

    struct IndexbufferData {
    public:
        ContextData* Context;

    private:
        UInt32 m_Count;

    private:
#if ST_RENDERER_DX11
        ID3D11Buffer* m_Buffer;
#endif  // ST_RENDERER_DX11

        friend class IndexbufferAPI;
    };
}  // namespace Sentinel