#pragma once

#include "Sentinel/Window/Window.h"

struct GLFWwindow;

namespace Sentinel {
    class GLFWWindow final: public Window<GLFWWindow> {
    public:
        GLFWWindow(const WindowProperties& props);

        void Init();
        void OnUpdate();
        void SetVSync(Bool enabled);
        void Shutdown();

        typedef int (*DragFn)();
        void SetDragLogic(DragFn function);

    private:
        GLFWwindow* m_Window;
    };
}  // namespace Sentinel
