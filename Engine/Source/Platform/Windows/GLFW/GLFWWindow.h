#pragma once

#include "Sentinel/Window/Window.h"

struct GLFWwindow;

namespace Sentinel {
    class GLFWWindow final: public Window {
    public:
        GLFWWindow(const WindowProperties& props);

        typedef int (*DragFn)();
        void SetDragLogic(DragFn function);

    private:
        void Init();
        void OnUpdate();
        void SetVSync(Bool enabled);
        void* GetNativeWindow() const { return m_Window; }
        void Shutdown();

    private:
        GLFWwindow* m_Window;
    };
}  // namespace Sentinel
