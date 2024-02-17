#ifndef COMMON_CORE_INTRINSICSDETECTION_H
#define COMMON_CORE_INTRINSICSDETECTION_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/CompilerDetection.h"

// Taken from eabase.h

#ifndef ST_SSE
    #if defined(ST_COMPILER_GCC) || defined(ST_COMPILER_CLANG)
        #if defined(__SSE3__)
            #define ST_SSE 3
        #elif defined(__SSE2__)
            #define ST_SSE 2
        #elif defined(__SSE__) && __SSE__
            #define ST_SSE 1
        #else
            #define ST_SSE 0
        #endif  // defined(__SSE3__)
    #elif defined(ST_SSE3) && ST_SSE3
        #define ST_SSE 3
    #elif defined(ST_SSE2) && ST_SSE2
        #define ST_SSE 2
    #elif defined(_WIN32) && defined(_MSC_FULL_VER) && !defined(__NOSSE__) && defined(_M_IX86_FP)
        #define ST_SSE _M_IX86_FP
    #elif defined(_WIN64)
        #define ST_SSE 2
    #else
        #define ST_SSE 0
    #endif  // defined(ST_COMPILER_GNUC) || defined(ST_COMPILER_CLANG)
#endif      // !ST_SSE

#ifndef ST_SSE2
    #if ST_SSE >= 2
        #define ST_SSE2 1
    #else
        #define ST_SSE2 0
    #endif  // ST_SSE >= 2
#endif      // !ST_SSE2

#ifndef ST_SSE3
    #if ST_SSE >= 3
        #define ST_SSE3 1
    #else
        #define ST_SSE3 0
    #endif  // ST_SSE >= 3
#endif      // !ST_SSE3

#ifndef ST_SSSE3
    #if defined __SSSE3__
        #define ST_SSSE3 1
    #else
        #define ST_SSSE3 0
    #endif  // defined __SSSE3__
#endif      // !ST_SSSE3

#ifndef ST_SSE4_1
    #if defined __SSE4_1__
        #define ST_SSE4_1 1
    #else
        #define ST_SSE4_1 0
    #endif  // defined __SSE4_1__
#endif      // !ST_SSE4_1

#ifndef ST_SSE4_2
    #if defined __SSE4_2__
        #define ST_SSE4_2 1
    #else
        #define ST_SSE4_2 0
    #endif  // defined __SSE4_2__
#endif      // !ST_SSE4_2

#ifndef ST_SSE4A
    #if defined __SSE4A__
        #define ST_SSE4A 1
    #else
        #define ST_SSE4A 0
    #endif  // defined __SSE4A__
#endif      // !ST_SSE4A

#ifndef ST_AVX
    #if defined __AVX2__
        #define ST_AVX 2
    #elif defined __AVX__
        #define ST_AVX 1
    #else
        #define ST_AVX 0
    #endif  // defined __AVX2__
#endif      // !ST_AVX

#ifndef ST_AVX2
    #if ST_AVX >= 2
        #define ST_AVX2 1
    #else
        #define ST_AVX2 0
    #endif  // ST_AVX >= 2
#endif      // !ST_AVX2

#endif  // !COMMON_CORE_INTRINSICSDETECTION_H
