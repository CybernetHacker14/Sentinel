#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

namespace Sentinel {
    class CPUInfo final {
    public:
        static CChar* GetCPUType();
        static UInt32 GetL1CacheLineSize();

    private:
        static Int32 HasCPUID();
        static void CalculateFeatures();

    private:
        inline static UInt32 CPUIDFeatures[4];
        inline static UInt32 CPUIDMaxFunction = 0;
        inline static Bool OSSavesYMM = false;

    private:
        inline CPUInfo() {}
    };
}  // namespace Sentinel
