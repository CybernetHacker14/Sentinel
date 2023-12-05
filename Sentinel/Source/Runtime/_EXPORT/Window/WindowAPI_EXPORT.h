#ifndef _EXPORT_WINDOW_WINDOWAPI_EXPORT_H
#define _EXPORT_WINDOW_WINDOWAPI_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "../ExportConditional.h"

typedef enum WindowMode WindowMode;
typedef struct WindowInterface WindowInterface;

ST_API void Sentinel_CreateInternalWindow(
    const char* titleBar, unsigned short width, unsigned short height, WindowMode mode, int resizeable);

ST_API void Sentinel_CreateExternalWindow(
    const char* titleBar,
    unsigned short width,
    unsigned short height,
    WindowMode mode,
    int resizeable,
    void (*initFn)(WindowInterface*, const char*),
    void (*updateFn)(),
    void (*deinitFn)());

ST_API int Sentinel_IsWindowRunning();

#endif  // !_EXPORT_WINDOW_WINDOWAPI_EXPORT_H
