#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Device/ContextData.h"

namespace Sentinel {
    struct VertexbufferLayoutData {
    public:
        ContextData* Context;

    protected:
        UInt32 m_Stride;

    private:
        friend class VertexbufferLayoutAPI;
    };
}  // namespace Sentinel