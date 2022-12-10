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
        m_CloseIndex = SubscribeToEvent(Sentinel::EventType::WindowClose, ST_BIND_FN(OnWindowClose));
        m_RunFunction = ST_BIND_FN(Run);

        Sentinel::WindowProperties props;
        props.Title = "Scribe";
        props.Width = 1280;
        props.Height = 720;
        props.Mode = Sentinel::WindowMode::BORDERLESSMAXIMIZED;
        props.FramebufferTransparency = false;

        m_Window = new Window::ScribeWindow(props);
        m_Window->SetEventCallback(ST_BIND_FN(RaiseEvent));

        m_BaseRenderer = new Rendering::ScribeRenderer(m_Window);
        m_ImGuiLayer = new Sentinel::ImGuiLayer(m_BaseRenderer->GetRenderingContext());
        m_ImGuiBase = new Rendering::ScribeImGuiBase(
            m_BaseRenderer->GetRenderingContext(), static_cast<Window::ScribeWindow*>(m_Window));

        m_BaseRenderer->OnAttach();
        m_ImGuiLayer->OnAttach();
        m_ImGuiBase->OnAttach();
    }

    Scribe::~Scribe() {
        UnsubscribeFromEvent(Sentinel::EventType::WindowClose, m_CloseIndex);
        delete m_Window;
    }

    void Scribe::Run() {
        while (m_Running) {
            if (!m_Minimized) {
                m_BaseRenderer->OnRender();
                m_ImGuiLayer->Begin();
                m_ImGuiBase->OnImGuiRender();
                m_ImGuiLayer->End();
                m_ImGuiBase->OnPostRender();
            }
            m_Window->OnUpdate();
            Sentinel::Input::OnUpdate();
        }
        m_ImGuiBase->OnDetach();
        m_ImGuiLayer->OnDetach();
        m_BaseRenderer->OnDetach();
        m_Window->Shutdown();
    }

    void Scribe::OnWindowClose(Sentinel::Event& event) {
        Sentinel::WindowCloseEvent e = *(event.Cast<Sentinel::WindowCloseEvent>());
        m_Running = false;
    }
}  // namespace Scribe