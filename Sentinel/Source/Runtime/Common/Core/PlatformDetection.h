#ifndef COMMON_CORE_PLATFORMDETECTION_H
#define COMMON_CORE_PLATFORMDETECTION_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

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
    #if defined(__ANDROID__) || defined(ANRDOID)
        #define ST_PLATFORM_ANDROID
        #error "Android is not supported"
    #else
        #define ST_PLATFORM_LINUX
        #error "Linux is not supported"
    #endif  // defined(__ANDROID__) || defined(ANRDOID)
#endif      // defined(__linux) || defined(linux)

#endif  // !COMMON_CORE_PLATFORMDETECTION_H
