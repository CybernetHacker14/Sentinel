#ifndef _EXPORT_EXPORTCONDITIONAL_H
#define _EXPORT_EXPORTCONDITIONAL_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

// Moved this from Common/Core/Definitions.h to here
// So that this remain isolated, and not used anywhere else
// Also removed dependency on CompilerDetection header

#if defined(_MSC_VER)
    #ifdef SENTINEL_DLL
        #define ST_API __declspec(dllexport)
    #else
        #define ST_API __declspec(dllimport)
    #endif  // SENTINEL_DLL
#elif defined(__GNUC__)
    #ifdef SENTINEL_DLL
        #define ST_API __attribute__((visibility("default")))
    #else
        #define ST_API
    #endif  // SENTINEL_DLL
#else
    #error "ST_API is not implemented for this compiler/platform"
#endif  // defined(_MSC_VER)

#endif  // !_EXPORT_EXPORTCONDITIONAL_H
