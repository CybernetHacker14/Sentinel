#ifndef LOGGING_LOG_H
#define LOGGING_LOG_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/Debug.h"

#ifdef ST_DEBUG
    #include "Common/Core/Definitions.h"

    #include <stdio.h>

    #define ANSI_COLOR_WHITE  "\x1b[97m"
    #define ANSI_COLOR_RED    "\x1b[31m"
    #define ANSI_COLOR_GREEN  "\x1b[32m"
    #define ANSI_COLOR_YELLOW "\x1b[33m"
    #define ANSI_COLOR_RESET  "\x1b[0m"

    #define BASE_PRINT_FORMAT(color, format, ...) \
        printf_s(color "%s:%04i: " format ANSI_COLOR_RESET "\n", ST_FUNC_SIG, __LINE__, __VA_ARGS__)

    #define ST_TERMINAL_TRACE(format, ...) BASE_PRINT_FORMAT(ANSI_COLOR_WHITE, format, __VA_ARGS__)
    #define ST_TERMINAL_INFO(format, ...)  BASE_PRINT_FORMAT(ANSI_COLOR_GREEN, format, __VA_ARGS__)
    #define ST_TERMINAL_WARN(format, ...)  BASE_PRINT_FORMAT(ANSI_COLOR_YELLOW, format, __VA_ARGS__)
    #define ST_TERMINAL_ERROR(format, ...) BASE_PRINT_FORMAT(ANSI_COLOR_RED, format, __VA_ARGS__)
#else
    #define ST_TERMINAL_TRACE(format, ...)
    #define ST_TERMINAL_INFO(format, ...)
    #define ST_TERMINAL_WARN(format, ...)
    #define ST_TERMINAL_ERROR(format, ...)
#endif  // ST_DEBUG

#endif  // !LOGGING_LOG_H
