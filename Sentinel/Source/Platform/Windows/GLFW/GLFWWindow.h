#ifndef WINDOWS_GLFW_GLFWWINDOW_H
#define WINDOWS_GLFW_GLFWWINDOW_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef struct WindowInterface WindowInterface;

void Init(WindowInterface* data, CChar* titleBarString);

void Update();

void Deinit();

#endif  // !WINDOWS_GLFW_GLFWWINDOW_H
