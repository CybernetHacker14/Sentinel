#pragma once

#include "Sentinel/Common/Core/PlatformDetection.h"

#ifdef ST_DEBUG
    #if defined(ST_PLATFORM_WINDOWS)
        #define ST_DEBUGBREAK() __debugbreak()
    #elif defined(ST_PLATFORM_LINUX)
        #include <signal.h>
        #define ST_DEBUGBREAK() raise(SIGTRAP)
    #endif  // defined(ST_PLATFORM_WINDOWS)
    #define ST_ENABLE_ASSERTS
#else
    #define ST_DEBUGBREAK()
#endif  // ST_DEBUG
