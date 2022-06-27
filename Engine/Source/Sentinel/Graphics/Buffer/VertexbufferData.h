#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    struct VertexbufferData {
    protected:
        void* m_Vertices;

    private:
        friend class VertexbufferAPI;
    };
}  // namespace Sentinel