#pragma once

#include "Sentinel/Window/Window.h"

namespace Sentinel {
    class GLFWWindow final: public Window {
    private:
        typedef int (*DragFn)();

    public:
        GLFWWindow(const WindowProperties& props);
        void SetDragLogic(DragFn function);

    private:
        void Init();
    };
}  // namespace Sentinel
