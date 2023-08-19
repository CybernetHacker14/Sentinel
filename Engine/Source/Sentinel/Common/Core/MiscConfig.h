#pragma once

#include "Sentinel/Common/Core/PlatformDetection.h"
#include "Sentinel/Common/Core/CompilerDetection.h"

#if defined(_MSC_VER)
    // DISABLE CERTAIN WARNINGS (taken from Spartan Engine)
    // 'type' : class 'type1' needs to have dll-interface to be used by clients of class 'type2'
    // https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4251?view=msvc-170
    #pragma warning(disable : 4251)

    // non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'
    // https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-2-c4275?view=msvc-170
    #pragma warning(disable : 4275)

    // no definition for inline function 'function'
    // https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4506?view=msvc-170
    #pragma warning(disable : 4506)

    // 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead.
    // https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-3-c4996?view=msvc-170
    #pragma warning(disable : 4996)

    // Caller failing to hold lock <lock> before calling function <func>
    // https://docs.microsoft.com/en-us/cpp/code-quality/c26110?view=msvc-170
    #pragma warning(disable : 26110)

    // 'conversion' conversion from 'type1' to 'type2', possible loss of data
    // https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-levels-3-and-4-c4244?view=msvc-170
    #pragma warning(disable : 4244)
#endif  // defined(_MSC_VER)

#ifdef ST_PLATFORM_WINDOWS
    #ifndef NOMINMAX
        // https://github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
        #define NOMINMAX
    #endif  // !NOMINMAX
#endif      // ST_PLATFORM_WINDOWS
