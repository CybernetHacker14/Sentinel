#include "Sandbox.h"

#include <Sentinel/Application/EntryPoint.h>
#include <Sentinel/Graphics/Camera/Camera.h>

#include <Platform/GLFW/GLFWWindow.h>

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
        m_RunFunction = ST_BIND_EVENT_FN(Sandbox::Run);

        //====================================WINDOW CREATION=======================================//
        Sentinel::WindowProperties props;
        props.Title = "Sandbox";
        props.Width = 1280;
        props.Height = 720;
        props.Mode = Sentinel::WindowMode::BORDERLESS;
        props.FramebufferTransparency = false;

        m_Window = Sentinel::CreateUniqueRef<Sentinel::GLFWWindow>(props);
        m_Window->SetEventCallback(ST_BIND_EVENT_FN(RaiseEvent));
        static_cast<Sentinel::GLFWWindow*>(m_Window.get())->SetDragLogic(&DragFunction);

        handle = glfwGetWin32Window(m_Window->GetNativeWindow<GLFWwindow>());

        //====================================DO NOT DELETE========================================//

        m_RenderLayer = new Rendering::RendererLayer(m_Window.get());
        PushLayer(m_RenderLayer);

        m_ImGuiLayer = new Sentinel::ImGuiLayer(m_RenderLayer->m_Context);
        PushOverlay(m_ImGuiLayer);
    }

    void Sandbox::Run() {
        while (m_Running) {
            if (!m_Minimized) {
                ProcessLayerUpdate();
                ProcessLayerRender();
                ProcessLayerImGuiRender();
                ProcessLayerPostRender();
            }
            m_Window->OnUpdate();
            Sentinel::Input::OnUpdate();
        }
        m_Window->Shutdown();
    }

    void Sandbox::ProcessLayerUpdate() {
        if (m_LayerStack.GetSize() == 0) return;
        for (Sentinel::Layer* layer: m_LayerStack) layer->OnUpdate();
    }

    void Sandbox::ProcessLayerRender() {
        if (m_LayerStack.GetSize() == 0) return;
        for (Sentinel::Layer* layer: m_LayerStack) layer->OnRender();
    }

    void Sandbox::ProcessLayerImGuiRender() {
        if (m_LayerStack.GetSize() == 0) return;

        m_ImGuiLayer->Begin();

        for (Sentinel::Layer* layer: m_LayerStack) layer->OnImGuiRender();

        m_ImGuiLayer->End();
    }

    void Sandbox::ProcessLayerPostRender() {
        if (m_LayerStack.GetSize() == 0) return;
        for (Sentinel::Layer* layer: m_LayerStack) layer->OnPostRender();
    }
}  // namespace Sandbox
