#include "Window/ScribeWindow.h"

#include <Sentinel/Logging/Log.h>
#include <Sentinel/Common/Core/Assert.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Sentinel {
    namespace WindowPFn {
        static Sentinel::UInt8 s_GLFWWindowCount = 0;
        static GLFWwindow* s_NativeWindow;

        static void OnUpdate() {
            glfwPollEvents();
        }

        static void VSync(Sentinel::Bool enabled) {
            glfwSwapInterval(enabled);
        }

        static void Shutdown() {
            glfwDestroyWindow(s_NativeWindow);
            --s_GLFWWindowCount;

            if (s_GLFWWindowCount == 0) { glfwTerminate(); }
        }

        static void* GetNative() {
            return s_NativeWindow;
        }
    }  // namespace WindowPFn
}  // namespace Sentinel

namespace Scribe {
    namespace Window {
        HWND handle;
        RECT rect;
        POINT cursor;

        int captionPadding = 25;

        static Sentinel::UInt8 s_GLFWWindowCount = 0;
        static Sentinel::Bool s_ShouldBlockDrag = false;

        int DragFunction() {
            if (s_ShouldBlockDrag) return false;

            GetWindowRect(handle, &rect);
            GetCursorPos(&cursor);

            return (cursor.x > rect.left && cursor.x < rect.right && cursor.y > rect.top &&
                    cursor.y < rect.top + captionPadding)
                       ? 1
                       : 0;
        }

        static void GLFWErrorCallback(int error, const char* description) {
            ST_TERMINAL_ERROR("GLFW Error:%i - %s", error, description);
            ST_BREAKPOINT_ASSERT(false, description)
        }

        void BlockTitleBarDrag(Sentinel::Bool value) {
            s_ShouldBlockDrag = value;
        }

        ScribeWindow::ScribeWindow(const Sentinel::WindowProperties& props) : Window(props) {
            Sentinel::WindowPFn::OnUpdateFn = Sentinel::WindowPFn::OnUpdate;
            Sentinel::WindowPFn::SetVSyncFn = Sentinel::WindowPFn::VSync;
            Sentinel::WindowPFn::ShutdownFn = Sentinel::WindowPFn::Shutdown;
            Sentinel::WindowPFn::GetNativeFn = Sentinel::WindowPFn::GetNative;

            SetDragLogic(DragFunction);
            Init();
        }

        void ScribeWindow::SetDragLogic(DragFn function) {
            SetMouseDragCheckFn(function);
        }

        void ScribeWindow::Init() {
            m_Data.Width = m_Properties.Width;
            m_Data.Height = m_Properties.Height;

            if (s_GLFWWindowCount == 0) {
                int success = glfwInit();
                ST_BREAKPOINT_ASSERT(success, "Could not initialize GLFW!");
                glfwSetErrorCallback(GLFWErrorCallback);
            }

            glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, m_Properties.Transparent ? 1 : 0);
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

            if (m_Properties.Mode == Sentinel::WindowMode::FULLSCREEN) {
                m_Window = glfwCreateWindow(
                    (Sentinel::Int32)m_Properties.Width,
                    (Sentinel::Int32)m_Properties.Height,
                    m_Properties.Title.C_Str(),
                    glfwGetPrimaryMonitor(),
                    nullptr);
            } else {
                m_Window = glfwCreateWindow(
                    (Sentinel::Int32)m_Properties.Width,
                    (Sentinel::Int32)m_Properties.Height,
                    m_Properties.Title.C_Str(),
                    nullptr,
                    nullptr);
                if (m_Properties.Mode == Sentinel::WindowMode::WINDOWEDMAXIMIZED ||
                    m_Properties.Mode == Sentinel::WindowMode::BORDERLESSMAXIMIZED) {
                    glfwMaximizeWindow(m_Window);
                    glfwGetWindowSize(m_Window, (int*)&(m_Data.Width), (int*)&(m_Data.Height));
                }
            }
            ++s_GLFWWindowCount;

            handle = glfwGetWin32Window(m_Window);

            // GLFWCallbacks
            glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, Sentinel::Int32 width, Sentinel::Int32 height) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;

                Sentinel::EventData eventData;
                eventData.UInt16[0] = width;
                eventData.UInt16[1] = height;
                Sentinel::EventsAPI::FireEvent(Sentinel::EventType::WindowResize, eventData);
            });

            glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                Sentinel::EventsAPI::FireEvent(Sentinel::EventType::WindowClose, Sentinel::EventData());
            });

            glfwSetKeyCallback(
                m_Window,
                [](GLFWwindow* window,
                   Sentinel::Int32 key,
                   Sentinel::Int32 scancode,
                   Sentinel::Int32 action,
                   Sentinel::Int32 mods) {
                    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                    Sentinel::EventData eventData;
                    eventData.UInt16[0] = key;
                    Sentinel::EventsAPI::FireEvent((Sentinel::EventType)(5 + action), eventData);
                });

            glfwSetCharCallback(m_Window, [](GLFWwindow* window, Sentinel::UInt32 keycode) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                Sentinel::EventData eventData;
                eventData.UInt16[0] = keycode;
                Sentinel::EventsAPI::FireEvent(Sentinel::EventType::KeyTyped, eventData);
            });

            glfwSetMouseButtonCallback(
                m_Window, [](GLFWwindow* window, Sentinel::Int32 button, Sentinel::Int32 action, Sentinel::Int32 mods) {
                    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                    Sentinel::EventData eventData;
                    eventData.UInt8[0] = button;
                    Sentinel::EventsAPI::FireEvent((Sentinel::EventType)(9 + action), eventData);
                });

            glfwSetScrollCallback(m_Window, [](GLFWwindow* window, Sentinel::Double xOffset, Sentinel::Double yOffset) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                Sentinel::EventData eventData;
                eventData.Float[0] = xOffset;
                eventData.Float[1] = yOffset;
                Sentinel::EventsAPI::FireEvent(Sentinel::EventType::MouseScrolled, eventData);
            });

            glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, Sentinel::Double xPos, Sentinel::Double yPos) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                Sentinel::EventData eventData;
                eventData.Float[0] = xPos;
                eventData.Float[1] = yPos;
                Sentinel::EventsAPI::FireEvent(Sentinel::EventType::MouseMoved, eventData);
            });

            glfwSetWindowUserPointer(m_Window, &m_Data);
        }

        void ScribeWindow::OnUpdate() {
            glfwPollEvents();
        }

        void ScribeWindow::SetVSync(Sentinel::Bool enabled) {
            glfwSwapInterval(enabled ? 1 : 0);
            m_Data.VSync = enabled;
        }

        void ScribeWindow::Shutdown() {
            glfwDestroyWindow(m_Window);
            --s_GLFWWindowCount;

            if (s_GLFWWindowCount == 0) { glfwTerminate(); }
        }

        Sentinel::Bool ScribeWindow::IsMaximized() {
            return glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED);
        }

        Sentinel::Bool ScribeWindow::IsMinimized() {
            return Sentinel::Bool();
        }

        Sentinel::Bool ScribeWindow::IsWindowed() {
            return Sentinel::Bool();
        }

        void ScribeWindow::Minimize() {
            glfwIconifyWindow(m_Window);
        }

        void ScribeWindow::Maximize() {
            glfwMaximizeWindow(m_Window);
        }

        void ScribeWindow::RestoreDown() {
            glfwRestoreWindow(m_Window);
        }

        void ScribeWindow::InvokeShutdown() {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(m_Window);

            Sentinel::EventsAPI::FireEvent(Sentinel::EventType::WindowClose, Sentinel::EventData());
        }
    }  // namespace Window
}  // namespace Scribe