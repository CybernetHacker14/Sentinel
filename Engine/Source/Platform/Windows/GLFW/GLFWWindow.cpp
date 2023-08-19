#include "stpch.h"
#include "Platform/Windows/GLFW/GLFWWindow.h"

#include "Sentinel/Common/Core/Assert.h"
#include "Sentinel/Logging/Log.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Sentinel {
    namespace WindowPFn {
        static UInt8 s_GLFWWindowCount = 0;
        static GLFWwindow* s_NativeWindow;

        static void OnUpdate() {
            glfwPollEvents();
        }

        static void VSync(Bool enabled) {
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

    static void GLFWErrorCallback(int error, const char* description) {
        ST_TERMINAL_ERROR("GLFW Error:%i - %s", error, description);
        ST_BREAKPOINT_ASSERT(false, description)
    }

    GLFWWindow::GLFWWindow(const WindowProperties& props) : Window(props) {
        WindowPFn::OnUpdateFn = Sentinel::WindowPFn::OnUpdate;
        WindowPFn::SetVSyncFn = Sentinel::WindowPFn::VSync;
        WindowPFn::ShutdownFn = Sentinel::WindowPFn::Shutdown;
        WindowPFn::GetNativeFn = Sentinel::WindowPFn::GetNative;
        Init();
    }

    void GLFWWindow::SetDragLogic(DragFn function) {
        SetMouseDragCheckFn(function);
    }

    void GLFWWindow::Init() {
        m_Data.Width = m_Properties.Width;
        m_Data.Height = m_Properties.Height;

        if (Sentinel::WindowPFn::s_GLFWWindowCount == 0) {
            int success = glfwInit();
            ST_BREAKPOINT_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, m_Properties.Transparent ? 1 : 0);
        glfwWindowHint(GLFW_DECORATED, m_Properties.Mode == WindowMode::BORDERLESS);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        if (m_Properties.Mode == WindowMode::FULLSCREEN) {
            Sentinel::WindowPFn::s_NativeWindow = glfwCreateWindow(
                (Int32)m_Properties.Width,
                (Int32)m_Properties.Height,
                m_Properties.Title.C_Str(),
                glfwGetPrimaryMonitor(),
                nullptr);
        } else {
            Sentinel::WindowPFn::s_NativeWindow = glfwCreateWindow(
                (Int32)m_Properties.Width, (Int32)m_Properties.Height, m_Properties.Title.C_Str(), nullptr, nullptr);
            if (m_Properties.Mode == WindowMode::WINDOWEDMAXIMIZED ||
                m_Properties.Mode == WindowMode::BORDERLESSMAXIMIZED) {
                glfwMaximizeWindow(Sentinel::WindowPFn::s_NativeWindow);
                glfwGetWindowSize(Sentinel::WindowPFn::s_NativeWindow, (int*)&(m_Data.Width), (int*)&(m_Data.Height));
            }
        }
        ++(Sentinel::WindowPFn::s_GLFWWindowCount);

        // GLFWCallbacks
        glfwSetWindowSizeCallback(
            Sentinel::WindowPFn::s_NativeWindow, [](GLFWwindow* window, Int32 width, Int32 height) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;

                // UniqueRef<Event> event(new WindowResizeEvent(width, height));
                // data.EventCallback(STL::move(event));
            });

        glfwSetWindowCloseCallback(Sentinel::WindowPFn::s_NativeWindow, [](GLFWwindow* window) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            // UniqueRef<Event> event(new WindowCloseEvent());
            // data.EventCallback(STL::move(event));
        });

        glfwSetKeyCallback(
            Sentinel::WindowPFn::s_NativeWindow,
            [](GLFWwindow* window, Int32 key, Int32 scancode, Int32 action, Int32 mods) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action) {
                    case GLFW_PRESS: {
                        // UniqueRef<Event> event(new KeyPressedEvent(key, 0));
                        // data.EventCallback(STL::move(event));
                        break;
                    }

                    case GLFW_RELEASE: {
                        // UniqueRef<Event> event(new KeyReleasedEvent(key));
                        // data.EventCallback(STL::move(event));
                        break;
                    }

                    case GLFW_REPEAT: {
                        // UniqueRef<Event> event(new KeyPressedEvent(key, 1));
                        // data.EventCallback(STL::move(event));
                        break;
                    }
                }
            });

        glfwSetCharCallback(Sentinel::WindowPFn::s_NativeWindow, [](GLFWwindow* window, UInt32 keycode) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            // UniqueRef<Event> event(new KeyTypedEvent(keycode));
            // data.EventCallback(STL::move(event));
        });

        glfwSetMouseButtonCallback(
            Sentinel::WindowPFn::s_NativeWindow, [](GLFWwindow* window, Int32 button, Int32 action, Int32 mods) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action) {
                    case GLFW_PRESS: {
                        // UniqueRef<Event> event(new MouseButtonPressedEvent(button));
                        // data.EventCallback(STL::move(event));
                        break;
                    }
                    case GLFW_RELEASE: {
                        // UniqueRef<Event> event(new MouseButtonReleasedEvent(button));
                        // data.EventCallback(STL::move(event));
                        break;
                    }
                }
            });

        glfwSetScrollCallback(
            Sentinel::WindowPFn::s_NativeWindow, [](GLFWwindow* window, Double xOffset, Double yOffset) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                // UniqueRef<Event> event(new MouseScrolledEvent(xOffset, yOffset));
                // data.EventCallback(STL::move(event));
            });

        glfwSetCursorPosCallback(Sentinel::WindowPFn::s_NativeWindow, [](GLFWwindow* window, Double xPos, Double yPos) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            // UniqueRef<Event> event(new MouseMovedEvent(xPos, yPos));
            // data.EventCallback(STL::move(event));
        });

        glfwSetWindowUserPointer(Sentinel::WindowPFn::s_NativeWindow, &m_Data);
    }
}  // namespace Sentinel
