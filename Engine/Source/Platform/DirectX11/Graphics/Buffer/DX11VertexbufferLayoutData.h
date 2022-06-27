#pragma once

#include "Sentinel/Graphics/Buffer/VertexbufferLayoutData.h"

struct ID3D11InputLayout;

namespace Sentinel {
    struct DX11VertexbufferLayoutData final: public VertexbufferLayoutData {
    private:
        ID3D11InputLayout* m_InputLayout;

    private:
        friend class DX11VertexbufferLayoutAPI;
    };
}  // namespace Sentinel