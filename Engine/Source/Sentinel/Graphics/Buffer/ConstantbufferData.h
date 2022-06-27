#pragma once

#include "Sentinel/Common/Common.h"

namespace Sentinel {
    enum class CBufferUsageType : UCChar { DEFAULT = 0, DYNAMIC = 1 };

    struct ConstantbufferData {
    protected:
        void* m_Data = nullptr;
        UInt32 m_Size = 0;
        CBufferUsageType m_UsageType = CBufferUsageType::DEFAULT;

    private:
        friend class ConstantbufferAPI;
    };
}  // namespace Sentinel