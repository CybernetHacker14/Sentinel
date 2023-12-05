#include "stpch.h"
#include "Window/Window.h"
#include "_EXPORT/Window/WindowAPI_EXPORT.h"
#include "_EXPORT/Window/WindowData_EXPORT.h"

#include "Common/Core/PlatformDetection.h"

#ifdef ST_PLATFORM_WINDOWS
    #include "GLFW/GLFWWindow.h"
#endif  // ST_PLATFORM_WINDOWS

static WindowInterface window;

ST_API void Sentinel_CreateInternalWindow(
    const char* titleBar, unsigned short width, unsigned short height, WindowMode mode, int resizeable) {
#ifdef ST_PLATFORM_WINDOWS
    window.init = Init;
    window.update = Update;
    window.deinit = Deinit;
#endif  // ST_PLATFORM_WINDOWS

    window.Width = width;
    window.Height = height;
    window.Mode = mode;

    window.Running = 1;
    window.VSync = 1;
    window.Transparent = 0;
    window.Resizable = resizeable;

    // Right now it's set as NORMAL
    // But it needs to initialized taking into account window.Mode as well
    window.State = NORMAL;

    window.init(&window, titleBar);
}

ST_API void Sentinel_CreateExternalWindow(
    const char* titleBar,
    unsigned short width,
    unsigned short height,
    WindowMode mode,
    int resizeable,
    void (*initFn)(WindowInterface*, const char*),
    void (*updateFn)(),
    void (*deinitFn)()) {
    window.init = initFn;
    window.update = updateFn;
    window.deinit = deinitFn;

    window.Width = width;
    window.Height = height;
    window.Mode = mode;

    window.Running = 1;
    window.VSync = 1;
    window.Transparent = 0;
    window.Resizable = resizeable;

    // Right now it's set as NORMAL
    // But it needs to initialized taking into account window.Mode as well
    window.State = NORMAL;

    window.init(&window, titleBar);
}

ST_API int Sentinel_IsWindowRunning() {
    return window.Running;
}

void Sentinel_Window_OnUpdate() {
    window.update();
}

void Sentinel_DestroyWindow() {
    window.deinit();
}

WindowInterface* Sentinel_GetWindow() {
    return &window;
}

void* Sentinel_Window_GetNativeHandle() {
    return window.nativeWindowHandle;
}

UShort Sentinel_Window_GetWidth() {
    return window.Width;
}

UShort Sentinel_Window_GetHeight() {
    return window.Height;
}
