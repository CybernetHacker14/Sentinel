#include "stpch.h"
#include "_EXPORT/Event/EventData_EXPORT.h"
#include "_EXPORT/Window/WindowData_EXPORT.h"
#include "Event/Event.h"
#include "GLFW/GLFWWindow.h"
#include "Window/Window.h"

#include <GLFW/glfw3.h>

static void ResizeCallback(GLFWwindow* window, Int width, Int height) {
    WindowData* sentinelWindow = (WindowData*)glfwGetWindowUserPointer(window);
    sentinelWindow->Width = (UShort)width;
    sentinelWindow->Height = (UShort)height;

    EventData data = {0};
    data.UInt16[0] = (UShort)width;
    data.UInt16[1] = (UShort)height;
    Sentinel_Event_FireEvent(WindowResize, data);
}

static void CloseCallback(GLFWwindow* window) {
}

void Sentinel_GLFWWindow_Init(WindowData* window, CChar* titleBarString) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, Sentinel_Window_IsWindowResizeable());

    GLFWwindow* glfwWindow = glfwCreateWindow(window->Width, window->Height, titleBarString, NULL, NULL);

    window->native = glfwWindow;

    glfwSetWindowSizeCallback(glfwWindow, ResizeCallback);
    glfwSetWindowCloseCallback(glfwWindow, CloseCallback);

    glfwSetWindowUserPointer(glfwWindow, window);
}

void Sentinel_GLFWWindow_Update(WindowData* window) {
    window->Running = !glfwWindowShouldClose((GLFWwindow*)(window->native));
    glfwPollEvents();
}

void Sentinel_GLFWWindow_Deinit(WindowData* window) {
    glfwDestroyWindow((GLFWwindow*)(window->native));
    glfwTerminate();
}

void Sentinel_GLFWWindow_Resize(WindowData* window, UShort width, UShort height) {
}
