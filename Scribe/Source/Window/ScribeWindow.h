#pragma once

#include <Sentinel.h>

struct GLFWwindow;

namespace Scribe {
    namespace Window {
        void BlockTitleBarDrag(Sentinel::Bool value);

        class ScribeWindow final: public Sentinel::Window {
        public:
            ScribeWindow(const Sentinel::WindowProperties& props);

            typedef int (*DragFn)();
            void SetDragLogic(DragFn function);

            Sentinel::Bool IsMaximized();
            Sentinel::Bool IsMinimized();
            Sentinel::Bool IsWindowed();

            void Minimize();
            void Maximize();
            void RestoreDown();
            void InvokeShutdown();

        private:
            void Init();
            void OnUpdate();
            void SetVSync(Sentinel::Bool enabled);
            void* GetNativeWindow() const { return m_Window; }
            void Shutdown();

        private:
            GLFWwindow* m_Window;
            Sentinel::Bool m_IsMouseOverImGui;
        };
    }  // namespace Window
}  // namespace Scribe