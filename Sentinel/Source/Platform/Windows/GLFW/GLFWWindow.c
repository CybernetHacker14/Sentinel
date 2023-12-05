#include "stpch.h"
#include "GLFW/GLFWWindow.h"
#include "Window/Window.h"

#include <GLFW/glfw3.h>

GLFWwindow* native;
WindowInterface* windowInterface;

void Init(WindowInterface* data, CChar* titleBarString) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    native = glfwCreateWindow(data->Width, data->Height, titleBarString, NULL, NULL);
    windowInterface = data;
    data->nativeWindowHandle = native;
}

void Update() {
    windowInterface->Running = !(glfwWindowShouldClose(native));
    glfwPollEvents();
}

void Deinit() {
    glfwDestroyWindow(native);
    glfwTerminate();
}
