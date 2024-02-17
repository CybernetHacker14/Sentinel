#include "stpch.h"
#include "Window/Window.h"
#include "_EXPORT/Window/WindowAPI_EXPORT.h"
#include "_EXPORT/Window/WindowData_EXPORT.h"

#include "Common/Core/PlatformDetection.h"

#include "Common/Core/Macros.h"

#ifdef ST_PLATFORM_WINDOWS
    #include "GLFW/GLFWWindow.h"
#endif  // ST_PLATFORM_WINDOWS

static WindowData internalWindow;

typedef struct WindowAPI {
    void (*init)(WindowData *, CChar *);
    void (*update)(WindowData *);
    void (*deinit)(WindowData *);
    void (*resize)(WindowData *, UShort, UShort);
} WindowAPI;

static WindowAPI windowAPI;

static WindowData *windowData;

ST_API void Sentinel_WindowAPI_CreateInternalWindow(
    const char *titleBar, unsigned short width, unsigned short height, WindowMode mode, int resizeable) {
#ifdef ST_PLATFORM_WINDOWS
    windowAPI.init = Sentinel_GLFWWindow_Init;
    windowAPI.update = Sentinel_GLFWWindow_Update;
    windowAPI.deinit = Sentinel_GLFWWindow_Deinit;
#endif  // ST_PLATFORM_WINDOWS

    windowData = &internalWindow;

    windowData->Width = width;
    windowData->Height = height;
    windowData->Mode = mode;

    windowData->Running = 1;
    windowData->VSync = 1;

    if (resizeable) windowData->Properties |= ST_BIT(0);  // Resizeable

    // Right now it's set as NORMAL
    // But it needs to initialized taking into account window.Mode as well
    windowData->State = NORMAL;

    windowAPI.init(windowData, titleBar);
}

ST_API void Sentinel_WindowAPI_CreateExternalWindow(
    WindowData *externalWindow,
    const char *titleBar,
    unsigned short width,
    unsigned short height,
    WindowMode mode,
    int resizeable,
    void (*initFn)(WindowData *, const char *),
    void (*updateFn)(WindowData *),
    void (*deinitFn)(WindowData *),
    void (*resizeFn)(WindowData *, unsigned short, unsigned short)) {
    windowAPI.init = initFn;
    windowAPI.update = updateFn;
    windowAPI.deinit = deinitFn;
    windowAPI.resize = resizeFn;

    windowData = externalWindow;

    windowData->Width = width;
    windowData->Height = height;
    windowData->Mode = mode;

    windowData->Running = 1;
    windowData->VSync = 1;

    if (resizeable) windowData->Properties |= ST_BIT(0);  // Resizeable

    // Right now it's set as NORMAL
    // But it needs to initialized taking into account window.Mode as well
    windowData->State = NORMAL;

    windowAPI.init(windowData, titleBar);
}

ST_API int Sentinel_WindowAPI_IsWindowRunning() {
    return windowData->Running;
}

void Sentinel_Window_OnUpdate() {
    windowAPI.update(windowData);
}

void Sentinel_Window_DestroyWindow() {
    windowAPI.deinit(windowData);
}

WindowData *Sentinel_Window_GetWindow() {
    return windowData;
}

void *Sentinel_Window_GetNativeHandle() {
    return windowData->native;
}

UShort Sentinel_Window_GetWidth() {
    return windowData->Width;
}

UShort Sentinel_Window_GetHeight() {
    return windowData->Height;
}

Int Sentinel_Window_IsWindowResizeable() {
    return windowData->Properties & ST_BIT(0);
}

Int Sentinel_Window_GetVSync() {
    return windowData->VSync;
}
