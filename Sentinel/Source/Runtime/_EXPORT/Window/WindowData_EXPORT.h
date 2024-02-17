#ifndef _EXPORT_WINDOW_WINDOWDATA_EXPORT_H
#define _EXPORT_WINDOW_WINDOWDATA_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef enum WindowMode {
    WINDOWED = 0,
    WINDOWED_MAXIMIZE = 1,
    BORDERLESS = 2,
    BORDERLESS_MAXIMIZE = 3,
    FULLSCREEN = 4
} WindowMode;

typedef enum WindowState { NORMAL = 0, MAXIMIZED = 1, MINIMIZED = 2 } WindowState;

// A platform agnostic window system data structure, which can be
// utilized by the engine
typedef struct WindowData {
    WindowMode Mode;

    unsigned short Width;
    unsigned short Height;

    int Running;
    int VSync;

    // 0 - Resizeable, 1 - Transparent
    int Properties;

    WindowState State;

    // For native, store the following pointers for each Backend
    // DX11 = GLFWwindow*
    void* native;
} WindowData;

#endif  // !_EXPORT_WINDOW_WINDOWDATA_EXPORT_H
