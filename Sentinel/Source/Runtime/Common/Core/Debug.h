#ifndef COMMON_CORE_DEBUG_H
#define COMMON_CORE_DEBUG_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/PlatformDetection.h"

#ifdef ST_DEBUG
    #if defined(ST_PLATFORM_WINDOWS)
        #define ST_DEBUGBREAK() __debugbreak()
    #else
        #include <signal.h>
        #define ST_DEBUGBREAK() raise(SIGTRAP)
    #endif  // defined(ST_PLATFORM_WINDOWS)
    #define ST_ENABLE_ASSERTS
#else
    #define ST_DEBUGBREAK()
#endif  // ST_DEBUG

#endif  // !COMMON_CORE_DEBUG_H
