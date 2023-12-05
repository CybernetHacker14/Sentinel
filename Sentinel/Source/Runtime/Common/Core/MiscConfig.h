#ifndef COMMON_CORE_MISCCONFIG_H
#define COMMON_CORE_MISCCONFIG_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/PlatformDetection.h"
#include "Common/Core/CompilerDetection.h"

#ifdef ST_DEBUG
    #ifdef ST_COMPILER_MSVC
    #endif  // ST_COMPILER_MSVC
#endif      // ST_DEBUG

#ifdef ST_DIST
    #ifdef ST_COMPILER_MSVC
        #pragma warning(disable : 6297)
        #pragma warning(disable : 4996)
        #pragma warning(disable : 4244)
    #endif  // ST_COMPILER_MSVC
#endif      // ST_DIST

#ifdef ST_PLATFORM_WINDOWS
    #ifndef NOMINMAX
        // https://github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
        #define NOMINMAX
    #endif  // !NOMINMAX
#endif      // ST_PLATFORM_WINDOWS

#endif  // !COMMON_CORE_MISCCONFIG_H
