#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    struct IndexbufferData {
    protected:
        UInt32 m_Count;

    private:
        friend class IndexbufferAPI;
    };
}  // namespace Sentinel