#pragma once

#include <Sentinel.h>

struct GLFWwindow;

namespace Scribe {
    namespace Window {
        static void BlockTitleBarDrag(Sentinel::Bool value);

        class EditorWindow final: public Sentinel::Window {
        public:
            EditorWindow(const Sentinel::WindowProperties& props);

            typedef int (*DragFn)();
            void SetDragLogic(DragFn function);

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