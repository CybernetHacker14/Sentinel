#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Device/ContextData.h"

namespace Sentinel {
    struct IndexbufferData {
    public:
        ContextData* Context;

    protected:
        UInt32 m_Count;

    private:
        friend class IndexbufferAPI;
    };
}  // namespace Sentinel