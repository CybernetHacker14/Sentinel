#pragma once

// Windows
#ifdef _WIN32
    #ifdef _WIN64
        #define ST_PLATFORM_WINDOWS
    #else
        #error "x86 builds are not supported"
    #endif  // _WIN64
#endif      // _WIN32

// Linux
#if defined(__linux) || defined(linux)
    #if defined(__ANDROID__) || defined(ANDROID)
        #define ST_PLATFORM_ANDROID
        #error "Android is not supported!"
    #else
        #define ST_PLATFORM_LINUX
        #error "Linux is not supported!"
    #endif  // defined(__ANDROID__) || defined(ANDROID)
#endif      // defined(__linux) || defined(linux)

// Apple
#if defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC == 1
        #define ST_PLATFORM_MACOS
        #error "MacOS is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #define ST_PLATFORM_IOS
        #error "iOS is not supported!"
    #else
        #error "Unknown Apple platform!"
    #endif  // TARGET_OS_MAC == 1
#endif      // defined(__APPLE__) || defined(__MACH__)
