#ifndef COMMON_CORE_ASSERT_H
#define COMMON_CORE_ASSERT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/Debug.h"
#include "Common/Core/Definitions.h"

// Some parts taken from eabase.h
#ifdef ST_ENABLE_ASSERTS
    #include <stdio.h>
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
    // Can add some sort of write to file and abort functionality like Spartan Engine
    #define ST_BREAKPOINT_ASSERT(expression, message)

#endif  // ST_ENABLE_ASSERTS

#endif  // !COMMON_CORE_ASSERT_H
