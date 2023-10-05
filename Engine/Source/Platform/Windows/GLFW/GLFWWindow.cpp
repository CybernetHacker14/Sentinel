#include "stpch.h"
#include "Platform/Windows/GLFW/GLFWWindow.h"

#include "Sentinel/Events/Event.h"
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
        m_UpdateFn = Sentinel::WindowPFn::OnUpdate;
        m_SetVSyncFn = Sentinel::WindowPFn::VSync;
        m_ShutdownFn = Sentinel::WindowPFn::Shutdown;
        m_GetNativeFn = Sentinel::WindowPFn::GetNative;
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

                EventData eventData;
                eventData.UInt16[0] = width;
                eventData.UInt16[1] = height;
                EventsAPI::FireEvent(EventType::WindowResize, eventData);
            });

        glfwSetWindowCloseCallback(Sentinel::WindowPFn::s_NativeWindow, [](GLFWwindow* window) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            EventsAPI::FireEvent(EventType::WindowClose, EventData());
        });

        glfwSetKeyCallback(
            Sentinel::WindowPFn::s_NativeWindow,
            [](GLFWwindow* window, Int32 key, Int32 scancode, Int32 action, Int32 mods) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                EventData eventData;
                eventData.UInt16[0] = key;
                EventsAPI::FireEvent((EventType)(5 + action), eventData);
            });

        glfwSetCharCallback(Sentinel::WindowPFn::s_NativeWindow, [](GLFWwindow* window, UInt32 keycode) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            EventData eventData;
            eventData.UInt16[0] = keycode;
            EventsAPI::FireEvent(EventType::KeyTyped, eventData);
        });

        glfwSetMouseButtonCallback(
            Sentinel::WindowPFn::s_NativeWindow, [](GLFWwindow* window, Int32 button, Int32 action, Int32 mods) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                EventData eventData;
                eventData.UInt8[0] = button;
                EventsAPI::FireEvent((EventType)(9 + action), eventData);
            });

        glfwSetScrollCallback(
            Sentinel::WindowPFn::s_NativeWindow, [](GLFWwindow* window, Double xOffset, Double yOffset) {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                EventData eventData;
                eventData.Float[0] = xOffset;
                eventData.Float[1] = yOffset;
                EventsAPI::FireEvent(EventType::MouseScrolled, eventData);
            });

        glfwSetCursorPosCallback(Sentinel::WindowPFn::s_NativeWindow, [](GLFWwindow* window, Double xPos, Double yPos) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            EventData eventData;
            eventData.Float[0] = xPos;
            eventData.Float[1] = yPos;
            EventsAPI::FireEvent(EventType::MouseMoved, eventData);
        });

        glfwSetWindowUserPointer(Sentinel::WindowPFn::s_NativeWindow, &m_Data);
        //glfwMakeContextCurrent(Sentinel::WindowPFn::s_NativeWindow);
    }
}  // namespace Sentinel
