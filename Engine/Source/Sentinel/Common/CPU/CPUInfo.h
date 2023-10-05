#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

namespace Sentinel {
    class CPUInfo final {
    public:
        static CChar* GetCPUType();
        static UInt8 GetL1CacheLineSize();

    private:
        inline CPUInfo() = default;

    private:
        static void Init();
        friend class Application;
    };
}  // namespace Sentinel
