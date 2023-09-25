#pragma once

#include "Sentinel/Common/Core/DataTypes.h"

// FSRM = fast rep movsb
#if defined(_WIN32) && defined(_M_X64) || defined(_M_IX86)
    #include <intrin.h>
    #define PROBABLE_FSRM_SUPPORTED_PLATFORM 1
#else
    #define PROBABLE_FSRM_SUPPORTED_PLATFORM 0
#endif  // defined(_WIN32) && defined(_M_X64) || defined(_M_IX86)

namespace Sentinel {
    namespace MemFuncUtils {
#if PROBABLE_FSRM_SUPPORTED_PLATFORM
        static Bool HasFSRMSupport = false;
#endif  // PROBABLE_FSRM_SUPPORTED_PLATFORM
    }   // namespace MemFuncUtils

    namespace MemFunctions {
        inline void Init() {
#if PROBABLE_FSRM_SUPPORTED_PLATFORM
            Int32 cpu_info[4];
            __cpuid(cpu_info, 7);
            MemFuncUtils::HasFSRMSupport = ((cpu_info[3] & (1 << 4)) != 0);
#endif  // PROBABLE_FSRM_SUPPORTED_PLATFORM
        }

        inline void Memcpy(void* dest, const void* src, Size_t n) {
#if PROBABLE_FSRM_SUPPORTED_PLATFORM
            if (MemFuncUtils::HasFSRMSupport) {
                __movsb((UChar*)dest, (const UChar*)src, n);
                return;
            }
#endif  // PROBABLE_FSRM_SUPPORTED_PLATFORM
            memcpy(dest, src, n);
        }

        inline void Memset(void* dest, Int32 val, Size_t n) {
#if PROBABLE_FSRM_SUPPORTED_PLATFORM
            if (MemFuncUtils::HasFSRMSupport) {
                __stosb((UChar*)dest, val, n);
                return;
            }
#endif  // PROBABLE_FSRM_SUPPORTED_PLATFORM
            memset(dest, val, n);
        }

    };  // namespace MemFunctions
}  // namespace Sentinel
