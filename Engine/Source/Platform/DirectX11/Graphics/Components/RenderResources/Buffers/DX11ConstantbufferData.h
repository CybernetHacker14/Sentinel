#pragma once

#include "Sentinel/Graphics/Components/RenderResources/Buffers/ConstantbufferData.h"

struct ID3D11Buffer;

namespace Sentinel {
    struct DX11ConstantbufferData final: public ConstantbufferData {
    private:
        ID3D11Buffer* m_Buffer;
        UInt32 m_BindSlot;

    private:
        friend class DX11ConstantbufferAPI;
    };
}  // namespace Sentinel