#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Common/Core/PlatformDetection.h"

namespace Sentinel {
    class CPUInfo {
    public:
        static const char* GetCPUType();
        static UInt32 GetL1CacheLineSize();

    private:
        static Int32 HasCPUID();
        static void CalculateFeatures();

    private:
        inline static UInt32 CPUIDFeatures[4];
        inline static UInt32 CPUIDMaxFunction = 0;
        inline static Bool OSSavesYMM = false;
    };
}  // namespace Sentinel
