#pragma once

#include "Sentinel/Window/Window.h"

struct GLFWwindow;

namespace Sentinel {
    class WindowsWindow final: public Window {
    public:
        WindowsWindow(const WindowProperties& props);

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
