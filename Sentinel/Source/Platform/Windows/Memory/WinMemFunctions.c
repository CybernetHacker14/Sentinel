#include "stpch.h"
#include "Common/Memory/MemFunctions.h"

#include <string.h>

// FSRM - fast rep movsb
#if defined(_M_X64)
    #include <intrin.h>
    #define PROBABLE_FSRM_SUPPORTED_PLATFORM 1
#else
    #define PROBABLE_FSRM_SUPPORTED_PLATFORM 0
#endif  // defined(_M_X64)

static Int HasFSRMSupport = 0;

void Sentinel_MemInit() {
#if PROBABLE_FSRM_SUPPORTED_PLATFORM
    Int cpu_info[4];
    __cpuid(cpu_info, 7);
    HasFSRMSupport = ((cpu_info[3] & (1 << 4)) != 0);
#endif  // PROBABLE_FSRM_SUPPORTED_PLATFORM
}

void Sentinel_Memcpy(void* dest, const void* src, ULLong n) {
#if PROBABLE_FSRM_SUPPORTED_PLATFORM
    if (HasFSRMSupport) {
        __movsb((UChar*)dest, (const UChar*)src, n);
        return;
    }

    memcpy(dest, src, n);
#else
    memcpy(dest, src, n);
#endif  // PROBABLE_FSRM_SUPPORTED_PLATFORM
}

void Sentinel_Memset(void* dest, Int val, ULLong n) {
#if PROBABLE_FSRM_SUPPORTED_PLATFORM
    if (HasFSRMSupport) {
        __stosb((UChar*)dest, val, n);
        return;
    }

    memset(dest, val, n);
#else
    memset(dest, val, n);
#endif  // PROBABLE_FSRM_SUPPORTED_PLATFORM
}
