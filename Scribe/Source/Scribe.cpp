#include "Scribe.h"

#include "Window/ScribeWindow.h"

#include "Renderer/ScribeRenderer.h"
#include "Renderer/ScribeImGuiBase.h"

#include <Sentinel/Application/EntryPoint.h>
#include <Sentinel/GUI/ImGui/ImGuiLayer.h>

// For launching the application with Nvidia card if available by default
extern "C" {
__declspec(dllexport) uint32_t NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace Scribe {
    Scribe::Scribe() {
        m_RunFunction = ST_BIND_EVENT_FN(Scribe::Run);

        Sentinel::WindowProperties props;
        props.Title = "Scribe";
        props.Width = 1280;
        props.Height = 720;
        props.Mode = Sentinel::WindowMode::BORDERLESSMAXIMIZED;
        props.FramebufferTransparency = false;

        m_Window = Sentinel::CreateUniqueRef<Window::ScribeWindow>(props);
        m_Window->SetEventCallback(ST_BIND_EVENT_FN(RaiseEvent));

        m_BaseRenderer = new Rendering::ScribeRenderer(m_Window.get());
        PushLayer(m_BaseRenderer);

        m_ImGuiLayer = new Sentinel::ImGuiLayer(m_BaseRenderer->GetRenderingContext());
        PushOverlay(m_ImGuiLayer);

        m_ImGuiBase = new Rendering::ScribeImGuiBase(m_BaseRenderer->GetRenderingContext());
        PushOverlay(m_ImGuiBase);
    }

    void Scribe::Run() {
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

    void Scribe::ProcessLayerUpdate() {
        if (m_LayerStack.GetSize() == 0) return;
        for (Sentinel::Layer* layer: m_LayerStack) layer->OnUpdate();
    }

    void Scribe::ProcessLayerRender() {
        if (m_LayerStack.GetSize() == 0) return;
        for (Sentinel::Layer* layer: m_LayerStack) layer->OnRender();
    }

    void Scribe::ProcessLayerImGuiRender() {
        if (m_LayerStack.GetSize() == 0) return;

        m_ImGuiLayer->Begin();

        for (Sentinel::Layer* layer: m_LayerStack) layer->OnImGuiRender();

        m_ImGuiLayer->End();
    }

    void Scribe::ProcessLayerPostRender() {
        if (m_LayerStack.GetSize() == 0) return;
        for (Sentinel::Layer* layer: m_LayerStack) layer->OnPostRender();
    }
}  // namespace Scribe