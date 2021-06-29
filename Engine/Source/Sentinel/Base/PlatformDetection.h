#pragma once

#ifdef _WIN32

// Windows
#ifdef _WIN64
#define ST_PLATFORM_WINDOWS
#else
#error "x86 builds are not supported"
#endif // _WIN64

// Apple
#elif defined(__APPLE__) || defined (__MACH__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#error "iOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
#define ST_PLATFORM_IOS
#error "iOS is not supported!"
#elif TARGET_OS_MAC == 1
#define ST_PLATFORM_MACOS
#error "MacOS is not supported!"
#else
#error "Unknown Application platform!"
#endif

// Linux
#elif defined(__ANDROID__)
#define ST_PLATFORM_LINUX
#error "Linux is not supported!"

#else
#error "Unknown platform!"

#endif // _WIN32
