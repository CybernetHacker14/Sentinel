#include "Window/EditorWindow.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Scribe {
    namespace Window {
        HWND handle;
        RECT rect;
        POINT cursor;

        int captionPadding = 25;

        static Sentinel::UInt8 s_GLFWWindowCount = 0;
        static Sentinel::Bool s_ShouldBlockDrag = false;

        int DragFunction() {
            GetWindowRect(handle, &rect);
            GetCursorPos(&cursor);

            return (cursor.x > rect.left && cursor.x < rect.right && cursor.y > rect.top &&
                    cursor.y < rect.top + captionPadding)
                       ? 1
                       : 0;
        }

        static void GLFWErrorCallback(int error, const char* description) {
            ST_ENGINE_ERROR("GLFW Error ({0}): {1}", error, description);
        }

        void BlockTitleBarDrag(Sentinel::Bool value) {
            s_ShouldBlockDrag = value;
        }

        EditorWindow::EditorWindow(const Sentinel::WindowProperties& props) : Window(props) {
            m_InitFunction = ST_BIND_EVENT_FN(Init);
            m_OnUpdateFunction = ST_BIND_EVENT_FN(OnUpdate);
            m_SetVSyncFunction = ST_BIND_EVENT_FN(SetVSync);
            m_GetNativeWindowFunction = ST_BIND_EVENT_FN(GetNativeWindow);
            m_ShutdownFunction = ST_BIND_EVENT_FN(Shutdown);

            SetDragLogic(DragFunction);
            Init();
        }

        void EditorWindow::SetDragLogic(DragFn function) {
            SetMouseDragCheckFn(function);
        }

        void EditorWindow::Init() {
            m_Data.Title = m_Properties.Title;
            m_Data.Width = m_Properties.Width;
            m_Data.Height = m_Properties.Height;

            ST_ENGINE_INFO(
                "Creating window {0} : {1}, {2}", m_Properties.Title.c_str(), m_Properties.Width, m_Properties.Height);

            if (s_GLFWWindowCount == 0) {
                int successs = glfwInit();
                ST_ENGINE_ASSERT(successs, "Could not initialize GLFW!");
                glfwSetErrorCallback(GLFWErrorCallback);
            }

            glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, m_Properties.FramebufferTransparency ? 1 : 0);
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

            if (m_Properties.Mode == Sentinel::WindowMode::FULLSCREEN) {
                m_Window = glfwCreateWindow(
                    (Sentinel::Int32)m_Properties.Width,
                    (Sentinel::Int32)m_Properties.Height,
                    m_Data.Title.c_str(),
                    glfwGetPrimaryMonitor(),
                    nullptr);
            } else {
                m_Window = glfwCreateWindow(
                    (Sentinel::Int32)m_Properties.Width,
                    (Sentinel::Int32)m_Properties.Height,
                    m_Data.Title.c_str(),
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

                Sentinel::UniqueRef<Sentinel::Event> event(new Sentinel::WindowResizeEvent(width, height));
                data.EventCallback(Sentinel::STL::move(event));
            });

            glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                Sentinel::UniqueRef<Sentinel::Event> event(new Sentinel::WindowCloseEvent());
                data.EventCallback(Sentinel::STL::move(event));
            });

            glfwSetKeyCallback(
                m_Window,
                [](GLFWwindow* window,
                   Sentinel::Int32 key,
                   Sentinel::Int32 scancode,
                   Sentinel::Int32 action,
                   Sentinel::Int32 mods) {
                    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                    switch (action) {
                        case GLFW_PRESS: {
                            Sentinel::UniqueRef<Sentinel::Event> event(new Sentinel::KeyPressedEvent(key, 0));
                            data.EventCallback(Sentinel::STL::move(event));
                            break;
                        }

                        case GLFW_RELEASE: {
                            Sentinel::UniqueRef<Sentinel::Event> event(new Sentinel::KeyReleasedEvent(key));
                            data.EventCallback(Sentinel::STL::move(event));
                            break;
                        }

                        case GLFW_REPEAT: {
                            Sentinel::UniqueRef<Sentinel::Event> event(new Sentinel::KeyPressedEvent(key, 1));
                            data.EventCallback(Sentinel::STL::move(event));
                            break;
                        }
                    }
                });

            glfwSetCharCallback(m_Window, [](GLFWwindow* window, Sentinel::UInt32 keycode) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                Sentinel::UniqueRef<Sentinel::Event> event(new Sentinel::KeyTypedEvent(keycode));
                data.EventCallback(Sentinel::STL::move(event));
            });

            glfwSetMouseButtonCallback(
                m_Window, [](GLFWwindow* window, Sentinel::Int32 button, Sentinel::Int32 action, Sentinel::Int32 mods) {
                    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                    switch (action) {
                        case GLFW_PRESS: {
                            Sentinel::UniqueRef<Sentinel::Event> event(new Sentinel::MouseButtonPressedEvent(button));
                            data.EventCallback(Sentinel::STL::move(event));
                            break;
                        }
                        case GLFW_RELEASE: {
                            Sentinel::UniqueRef<Sentinel::Event> event(new Sentinel::MouseButtonReleasedEvent(button));
                            data.EventCallback(Sentinel::STL::move(event));
                            break;
                        }
                    }
                });

            glfwSetScrollCallback(m_Window, [](GLFWwindow* window, Sentinel::Double xOffset, Sentinel::Double yOffset) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                Sentinel::UniqueRef<Sentinel::Event> event(new Sentinel::MouseScrolledEvent(xOffset, yOffset));
                data.EventCallback(Sentinel::STL::move(event));
            });

            glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, Sentinel::Double xPos, Sentinel::Double yPos) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                Sentinel::UniqueRef<Sentinel::Event> event(new Sentinel::MouseMovedEvent(xPos, yPos));
                data.EventCallback(Sentinel::STL::move(event));
            });

            glfwSetWindowUserPointer(m_Window, &m_Data);
        }

        void EditorWindow::OnUpdate() {
            glfwPollEvents();
        }

        void EditorWindow::SetVSync(Sentinel::Bool enabled) {
            glfwSwapInterval(enabled ? 1 : 0);
            m_Data.VSync = enabled;
        }

        void EditorWindow::Shutdown() {
            glfwDestroyWindow(m_Window);
            --s_GLFWWindowCount;

            if (s_GLFWWindowCount == 0) { glfwTerminate(); }
        }
    }  // namespace Window
}  // namespace Scribe