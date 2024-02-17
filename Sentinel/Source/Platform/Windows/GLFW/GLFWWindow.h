#ifndef WINDOWS_GLFW_GLFWWINDOW_H
#define WINDOWS_GLFW_GLFWWINDOW_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef struct WindowData WindowData;

void Sentinel_GLFWWindow_Init(WindowData* window, CChar* titleBarString);

void Sentinel_GLFWWindow_Update(WindowData* window);

void Sentinel_GLFWWindow_Deinit(WindowData* window);

void Sentinel_GLFWWindow_Resize(WindowData* window, UShort width, UShort height);

#endif  // !WINDOWS_GLFW_GLFWWINDOW_H
