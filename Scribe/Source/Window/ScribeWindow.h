#pragma once

#include <Sentinel.h>
#include <Sentinel/Window/Window.h>
#include <Sentinel/Common/Strings/StringView.h>

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
            void Shutdown();

        private:
            Sentinel::Bool m_IsMouseOverImGui;
        };
    }  // namespace Window
}  // namespace Scribe