#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

namespace Sentinel {
    class MemFunctions final {
    public:
        static void Memcpy(void* dest, const void* src, Size_t n);
        static void Memset(void* dest, Int32 val, Size_t n);

    private:
        inline MemFunctions() = default;

    private:
        static void Init();
        friend class Application;
    };
}  // namespace Sentinel
