#include "stpch.h"
#include "Platform/GLFW/GLFWWindow.h"

#include "Sentinel/Events/Categories/WindowEvent.h"
#include "Sentinel/Events/Categories/MouseEvent.h"
#include "Sentinel/Events/Categories/KeyEvent.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Sentinel {
    static UInt8 s_GLFWWindowCount = 0;

    static void GLFWErrorCallback(int error, const char* description) {
        ST_ENGINE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    GLFWWindow::GLFWWindow(const WindowProperties& props) : Window(props) {
        m_InitFunction = ST_BIND_EVENT_FN(Init);
        m_OnUpdateFunction = ST_BIND_EVENT_FN(OnUpdate);
        m_SetVSyncFunction = ST_BIND_EVENT_FN(SetVSync);
        m_GetNativeWindowFunction = ST_BIND_EVENT_FN(GetNativeWindow);
        m_ShutdownFunction = ST_BIND_EVENT_FN(Shutdown);

        Init();
    }

    void GLFWWindow::SetDragLogic(DragFn function) {
        SetMouseDragCheckFn(function);
    }

    void GLFWWindow::Init() {
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

        if (m_Properties.Mode == WindowMode::FULLSCREEN) {
            m_Window = glfwCreateWindow(
                (Int32)m_Properties.Width,
                (Int32)m_Properties.Height,
                m_Data.Title.c_str(),
                glfwGetPrimaryMonitor(),
                nullptr);
        } else {
            m_Window = glfwCreateWindow(
                (Int32)m_Properties.Width, (Int32)m_Properties.Height, m_Data.Title.c_str(), nullptr, nullptr);
            if (m_Properties.Mode == WindowMode::WINDOWEDMAXIMIZED ||
                m_Properties.Mode == WindowMode::BORDERLESSMAXIMIZED) {
                glfwMaximizeWindow(m_Window);
                glfwGetWindowSize(m_Window, (int*)&(m_Data.Width), (int*)&(m_Data.Height));
            }
        }
        ++s_GLFWWindowCount;

        // GLFWCallbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, Int32 width, Int32 height) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            UniqueRef<Event> event(new WindowResizeEvent(width, height));
            data.EventCallback(STL::move(event));
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            UniqueRef<Event> event(new WindowCloseEvent());
            data.EventCallback(STL::move(event));
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, Int32 key, Int32 scancode, Int32 action, Int32 mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    UniqueRef<Event> event(new KeyPressedEvent(key, 0));
                    data.EventCallback(STL::move(event));
                    break;
                }

                case GLFW_RELEASE: {
                    UniqueRef<Event> event(new KeyReleasedEvent(key));
                    data.EventCallback(STL::move(event));
                    break;
                }

                case GLFW_REPEAT: {
                    UniqueRef<Event> event(new KeyPressedEvent(key, 1));
                    data.EventCallback(STL::move(event));
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, UInt32 keycode) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            UniqueRef<Event> event(new KeyTypedEvent(keycode));
            data.EventCallback(STL::move(event));
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, Int32 button, Int32 action, Int32 mods) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    UniqueRef<Event> event(new MouseButtonPressedEvent(button));
                    data.EventCallback(STL::move(event));
                    break;
                }
                case GLFW_RELEASE: {
                    UniqueRef<Event> event(new MouseButtonReleasedEvent(button));
                    data.EventCallback(STL::move(event));
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, Double xOffset, Double yOffset) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            UniqueRef<Event> event(new MouseScrolledEvent(xOffset, yOffset));
            data.EventCallback(STL::move(event));
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, Double xPos, Double yPos) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            UniqueRef<Event> event(new MouseMovedEvent(xPos, yPos));
            data.EventCallback(STL::move(event));
        });

        glfwSetWindowUserPointer(m_Window, &m_Data);
    }

    void GLFWWindow::OnUpdate() {
        glfwPollEvents();
    }

    void GLFWWindow::SetVSync(Bool enabled) {
        glfwSwapInterval(enabled ? 1 : 0);
        m_Data.VSync = enabled;
    }

    void GLFWWindow::Shutdown() {
        glfwDestroyWindow(m_Window);
        --s_GLFWWindowCount;

        if (s_GLFWWindowCount == 0) { glfwTerminate(); }
    }
}  // namespace Sentinel
