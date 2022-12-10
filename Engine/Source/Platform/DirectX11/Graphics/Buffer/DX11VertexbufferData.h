#pragma once

#include "Sentinel/Graphics/Buffer/VertexbufferData.h"

struct ID3D11Buffer;

namespace Sentinel {
    struct DX11VertexbufferData final: public VertexbufferData {
    private:
        ID3D11Buffer* m_Buffer;

    private:
        friend class DX11VertexbufferAPI;
    };
}  // namespace Sentinel