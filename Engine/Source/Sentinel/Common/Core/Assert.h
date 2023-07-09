#pragma once

#include "Sentinel/Common/Core/Debug.h"
#include "Sentinel/Common/Core/Definitions.h"

// Some parts taken from eabase.h
#ifdef ST_ENABLE_ASSERTS
    #include <assert.h>
    #include <stdio.h>

    #if defined(_MSC_VER) && (_MSC_VER >= 1600) && defined(__cplusplus)
    // static_assert is defined by the compiler for both C and C++.
    #elif !defined(__cplusplus) && defined(ST_PLATFORM_ANDROID) && \
        ((defined(__STDC_VERSION__) && __STDC_VERSION__ < 201100L) || !defined(__STDC_VERSION__))
        // AndroidNDK does not support static_assert despite claiming it's a C11 compiler
        #define NEED_CUSTOM_STATIC_ASSERT
    #elif defined(__clang__) && defined(__cplusplus)
        // We need to separate these checks on a new line, as the pre-processor on other compilers will fail on the
        // _has_feature macros
        #if !(__has_feature(cxx_static_assert) || __has_extension(cxx_static_assert))
            #define NEED_CUSTOM_STATIC_ASSERT
        #endif
    #elif defined(__GNUC__) && \
        (defined(__GXX_EXPERIMENTAL_CXX0X__) || (defined(__cplusplus) && (__cplusplus >= 201103L)))
    // static_assert is defined by the compiler.
    #elif defined(__EDG_VERSION__) && (__EDG_VERSION__ >= 401) && defined(EA_COMPILER_CPP11_ENABLED)
    // static_assert is defined by the compiler.
    #elif !defined(__cplusplus) && defined(__GLIBC__) && defined(__USE_ISOC11)
    // static_assert is defined by the compiler.
    #elif !defined(__cplusplus) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201100L
    // static_assert is defined by the compiler.
    #else
        #define NEED_CUSTOM_STATIC_ASSERT
    #endif  // defined(_MSC_VER) && (_MSC_VER >= 1600) && defined(__cplusplus)

    #if NEED_CUSTOM_STATIC_ASSERT
        #ifdef __GNUC__
            #define ST_STATIC_ASSERT_UNUSED __attribute__((unused))
        #else
            #define ST_STATIC_ASSERT_UNUSED
        #endif  // __GNUC__

        #define TOKEN_PASTE(a, b) a##b
        #define CONCATENATE(a, b) TOKEN_PASTE(a, b)

        #if defined(__COUNTER__)
            #define static_assert(expression, description) \
                typedef char CONCATENATE(                  \
                    compileTimeAssert, __COUNTER__)[((expression) != 0) ? 1 : -1] ST_STATIC_ASSERT_UNUSED
        #else
            #define static_assert(expression, description) \
                typedef char CONCATENATE(                  \
                    compileTimeAssert, __LINE__)[((expression) != 0) ? 1 : -1] ST_STATIC_ASSERT_UNUSED
        #endif  // defined(__COUNTER__)

    #endif  // NEED_CUSTOM_STATIC_ASSERT

    #define ST_STATIC_ASSERT(expression) static_assert(expression)

    // Instead of linking to a Log class, gonna use the standard functions here.
    // This need not be extremely fancy, it's a terminal output macro only
    #define ST_BREAKPOINT_ASSERT(expression, message) \
        if (!(##expression)) {                        \
            printf_s(                                 \
                "\x1b[31m"                            \
                "Assertion failed.\n"                 \
                "Expression: %s\n"                    \
                "File: %s\n"                          \
                "Line: %i\n"                          \
                "Function: %s\n"                      \
                "Signature: %s\n"                     \
                "\x1b[0m",                            \
                #expression,                          \
                __FILE__,                             \
                __LINE__,                             \
                __FUNCTION__,                         \
                ST_FUNC_SIG);                         \
            printf_s(#message);                       \
            ST_DEBUGBREAK();                          \
        }

#else

    #define ST_STATIC_ASSERT(expression) static_assert(expression)
    // Can add some sort of write to file and abort functionality like Spartan Engine
    #define ST_BREAKPOINT_ASSERT(expression, message)

#endif  // ST_ENABLE_ASSERTS
