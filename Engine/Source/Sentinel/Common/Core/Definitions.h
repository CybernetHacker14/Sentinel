#pragma once

#include "Sentinel/Common/Core/PlatformDetection.h"
#include "Sentinel/Common/Core/CompilerDetection.h"

#if defined(ST_COMPILER_MSVC)
    #define ST_FUNC_SIG __FUNCSIG__
#elif defined(ST_COMPILER_GCC || ST_COMPILER_CLANG || ST_COMPILER_GCC_LLVM)
    #define ST_FUNC_SIG __PRETTY_FUNCTION__
#else
    #define ST_FUNC_SIG "ST_FUNC_SIG unknown!"
#endif  // defined(ST_COMPILER_MSVC)

#if (defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    #define ST_LITTLE_ENDIAN
#elif (defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    #define ST_BIG_ENDIAN
#elif defined(ST_PLATFORM_WINDOWS)
    #define ST_LITTLE_ENDIAN
#else
    #error "Endian detection needs to be set up for your compiler"
#endif  // (defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)

#define ST_POWERS_OF_10(factor)                                                                                     \
    factor * 10, (factor)*100, (factor)*1000, (factor)*10000, (factor)*100000, (factor)*1000000, (factor)*10000000, \
        (factor)*100000000, (factor)*1000000000

/*
// ====== Taken from fmt library ======//

#ifdef ST_COMPILER_MSVC
namespace Sentinel {
    inline Int32 clz(UInt32 x) {
        ULong r = 0;
        _BitScanReverse(&r, x);
        return 31 ^ static_cast<Int32>(r);
    }

    inline Int32 clzll(UInt64 x) {
        ULong r = 0;
        _BitScanReverse64(&r, x);
        return 63 ^ static_cast<Int32>(r);
    }
}  // namespace Sentinel

    #define ST_BUILTIN_CLZ(n)   Sentinel::clz(n)
    #define ST_BUILTIN_CLZLL(n) Sentinel::clzll(n)

#elif ST_COMPILER_GCC
    #define ST_BUILTIN_CLZ(n)   __builtin_clz(n)
    #define ST_BUILTIN_CLZLL(n) __builtin_clzll(n)
#else
    #define ST_BUILTIN_CLZ(n)   0
    #define ST_BUILTIN_CLZLL(n) 0
#endif  // ST_COMPILER_MSVC

// ====== \Taken from fmt library =====//
*/