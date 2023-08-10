#include "Sandbox.h"

#include <Sentinel/Application/EntryPoint.h>
#include <Sentinel/Graphics/Camera/Camera.h>

#include <Platform/Windows/GLFW/GLFWWindow.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

extern "C" {
__declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#ifdef ST_PLATFORM_WINDOWS
    #include <Windows.h>
#endif  // ST_PLATFORM_WINDOWS

HWND handle;
RECT rect;
POINT cursor;

int captionPadding = 25;

int DragFunction() {
    GetWindowRect(handle, &rect);
    GetCursorPos(&cursor);

    return (cursor.x > rect.left && cursor.x < rect.right && cursor.y > rect.top &&
            cursor.y < rect.top + captionPadding)
               ? 1
               : 0;
}

namespace Sandbox {
    Sandbox::Sandbox() {
        m_RunFunction = ST_BIND_FN(Sandbox::Run);

        //====================================WINDOW CREATION=======================================//
        Sentinel::WindowProperties props;
        props.Title = "Sandbox";
        props.Width = 1280;
        props.Height = 720;
        props.Mode = Sentinel::WindowMode::WINDOWED;
        props.Transparent = false;

        m_Window = new Sentinel::GLFWWindow(props);
        m_Window->SetEventCallback(ST_BIND_FN(RaiseEvent));
        static_cast<Sentinel::GLFWWindow*>(m_Window)->SetDragLogic(&DragFunction);

        handle = glfwGetWin32Window(m_Window->GetNativeWindow<GLFWwindow>());

        //====================================DO NOT DELETE========================================//

        m_RenderLayer = new Rendering::RendererLayer(m_Window);
        // m_ImGuiLayer = new Sentinel::ImGuiLayer(m_RenderLayer->m_Context);
        // m_ImGuiLayer->OnAttach();
        m_RenderLayer->OnAttach();
    }

    void Sandbox::Run() {
        while (m_Running) {
            if (!m_Minimized) {
                m_RenderLayer->OnUpdate();
                m_RenderLayer->OnRender();
                //m_ImGuiLayer->Begin();
                //m_ImGuiLayer->End();
                m_RenderLayer->OnPostRender();
            }
            m_Window->OnUpdate();
            Sentinel::Input::OnUpdate();
        }
        //m_ImGuiLayer->OnDetach();
        m_RenderLayer->OnDetach();
        m_Window->Shutdown();
    }
}  // namespace Sandbox
