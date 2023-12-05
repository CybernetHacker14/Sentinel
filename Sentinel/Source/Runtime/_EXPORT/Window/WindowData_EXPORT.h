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

#endif  // !_EXPORT_WINDOW_WINDOWDATA_EXPORT_H
