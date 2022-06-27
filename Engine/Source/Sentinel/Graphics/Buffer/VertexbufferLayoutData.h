#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    struct VertexbufferLayoutData {
    protected:
        UInt32 m_Stride;

    private:
        friend class VertexbufferLayoutAPI;
    };
}  // namespace Sentinel