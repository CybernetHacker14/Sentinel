#ifndef COMMON_CORE_COMPILERDETECTION_H
#define COMMON_CORE_COMPILERDETECTION_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#if defined(_MSC_VER)
    #define ST_COMPILER_MSVC
#endif  // defined(_MSC_VER)

#if defined(__GNUC__)
    #if defined(__llvm__)
        #if defined(__clang__)
            #define ST_COMPILER_CLANG
        #else
            #define ST_COMPILER_GCC_LLVM
        #endif  // defined(__clang__)
    #elif defined(__MINGW32__) || defined(__MINGW64__)
        #define ST_COMPILER_MINGW
    #else
        #define ST_COMPILER_GCC
    #endif  // defined(__llvm__)

#endif  // defined(__GNUC__)

#endif  // !COMMON_CORE_COMPILERDETECTION_H
