#pragma once

#include "Sentinel/Common/Common.h"

#if ST_RENDERER_DX11
struct ID3D11Buffer;
#endif  // ST_RENDERER_DX11

namespace Sentinel {
    enum class CBufferUsageType : Bool { DEFAULT = 0, DYNAMIC = 1 };
    struct ContextData;

    struct ConstantbufferData {
    public:
        ContextData* Context;

    private:
        void* m_Data = nullptr;
        UInt32 m_Size = 0;
        CBufferUsageType m_UsageType = CBufferUsageType::DEFAULT;

    private:
#if ST_RENDERER_DX11
        ID3D11Buffer* m_Buffer;
        UInt32 m_BindSlot;
#endif  // ST_RENDERER_DX11

        friend class ConstantbufferAPI;
    };
}  // namespace Sentinel