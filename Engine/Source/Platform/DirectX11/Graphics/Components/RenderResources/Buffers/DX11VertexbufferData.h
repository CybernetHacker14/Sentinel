#pragma once

#include "Sentinel/Graphics/Components/RenderResources/Buffers/VertexbufferData.h"

struct ID3D11Buffer;

namespace Sentinel {
    struct DX11VertexbufferData final: public VertexbufferData {
    private:
        ID3D11Buffer* m_VertexbufferPtr;

    private:
        friend class DX11VertexbufferAPI;
    };
}  // namespace Sentinel