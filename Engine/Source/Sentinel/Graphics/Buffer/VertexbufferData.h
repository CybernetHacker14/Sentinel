#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Device/ContextData.h"

namespace Sentinel {
    struct VertexbufferData {
    public:
        ContextData* Context;

    protected:
        void* m_Vertices;

    private:
        friend class VertexbufferAPI;
    };
}  // namespace Sentinel