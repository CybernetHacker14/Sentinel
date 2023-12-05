#ifndef COMMON_CORE_DEFINITIONS_H
#define COMMON_CORE_DEFINITIONS_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/CompilerDetection.h"
#include "Common/Core/PlatformDetection.h"

#if defined(ST_COMPILER_MSVC)
    #define ST_FUNC_SIG __FUNCSIG__
#elif defined(ST_COMPILER_GCC || ST_COMPILER_CLANG || ST_COMPILER_GCC_LLVM)
    #define ST_FUNC_SIG __PRETTY_FUNCTION__
#else
    #define ST_FUNC_SIG 0
    #error "ST_FUNC_SIG unknown!"
#endif  // defined(ST_COMPILER_MSVC)

#if (defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    #define ST_LITTLE_ENDIAN
#elif (defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
    #define ST_BIG_ENDIAN
#elif defined(ST_PLATFORM_WINDOWS)
    #define ST_LITTLE_ENDIAN
#else
    #error "Endian detection needs to be set up for you compiler"
#endif  // (defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)

#endif  // !COMMON_CORE_DEFINITIONS_H
