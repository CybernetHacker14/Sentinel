#pragma once

#include "Sentinel/Graphics/Components/RenderResources/Buffers/IndexbufferData.h"

struct ID3D11Buffer;

namespace Sentinel {
    struct DX11IndexbufferData final: public IndexbufferData {
    private:
        ID3D11Buffer* m_IndexbufferPtr;

    private:
        friend class DX11IndexbufferAPI;
    };
}  // namespace Sentinel