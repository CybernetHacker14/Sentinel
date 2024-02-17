#ifndef WINDOW_WINDOW_H
#define WINDOW_WINDOW_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef enum WindowMode WindowMode;
typedef enum WindowState WindowState;
typedef struct WindowData WindowData;

void Sentinel_Window_OnUpdate();

void Sentinel_Window_DestroyWindow();

void* Sentinel_Window_GetNativeHandle();

UShort Sentinel_Window_GetWidth();

UShort Sentinel_Window_GetHeight();

Int Sentinel_Window_IsWindowResizeable();

Int Sentinel_Window_GetVSync();

#endif  // !WINDOW_WINDOW_H
