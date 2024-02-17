#ifndef _EXPORT_WINDOW_WINDOWAPI_EXPORT_H
#define _EXPORT_WINDOW_WINDOWAPI_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "../ExportConditional.h"

typedef enum WindowMode WindowMode;
typedef struct WindowData WindowData;

ST_API void Sentinel_WindowAPI_CreateInternalWindow(
    const char* titleBar, unsigned short width, unsigned short height, WindowMode mode, int resizeable);

ST_API void Sentinel_WindowAPI_CreateExternalWindow(
    WindowData* externalWindow,
    const char* titleBar,
    unsigned short width,
    unsigned short height,
    WindowMode mode,
    int resizeable,
    void (*initFn)(WindowData*, const char*),
    void (*updateFn)(WindowData*),
    void (*deinitFn)(WindowData*),
    void (*resizeFn)(WindowData*, unsigned short, unsigned short));

ST_API int Sentinel_WindowAPI_IsWindowRunning();

#endif  // !_EXPORT_WINDOW_WINDOWAPI_EXPORT_H
