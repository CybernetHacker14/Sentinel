#ifndef WINDOW_WINDOW_H
#define WINDOW_WINDOW_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef enum WindowMode WindowMode;
typedef enum WindowState WindowState;

// Name alternatives - WindowBackend
typedef struct WindowInterface {
    WindowMode Mode;

    UShort Width;
    UShort Height;

    Int VSync;
    Int Running;
    Int Resizable;
    Int Transparent;

    WindowState State;

    void* nativeWindowHandle;

    void (*init)(struct WindowInterface*, CChar*);
    void (*update)();
    void (*deinit)();
} WindowInterface;

void Sentinel_Window_OnUpdate();

void Sentinel_DestroyWindow();

WindowInterface* Sentinel_GetWindow();

void* Sentinel_Window_GetNativeHandle();

UShort Sentinel_Window_GetWidth();

UShort Sentinel_Window_GetHeight();

#endif  // !WINDOW_WINDOW_H
