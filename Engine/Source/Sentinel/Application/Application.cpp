#include "stpch.h"
#include "Sentinel/Application/Application.h"
#include "Sentinel/Events/Categories/WindowEvent.h"
#include "Sentinel/Events/Categories/KeyEvent.h"
#include "Sentinel/Input/Input.h"
#include "Sentinel/Input/KeyCodes.h"

#include "Sentinel/Math/Math.h"

#include "Sentinel/Memory/PoolAllocator.h"
#include "Sentinel/Common/CPU/CPUInfo.h"

namespace Sentinel {
    Application* Application::s_Instance = nullptr;

    Application::Application(const STL::string& name) {
        ST_ENGINE_ASSERT(!s_Instance, "Application instance already exist!");
        s_Instance = this;

        // Set frame backings definitions

        WindowProperties props;
        props.Title = name;
        props.Width = 1280;
        props.Height = 720;
        props.Mode = WindowMode::WINDOWED;
        props.FramebufferTransparency = false;

        ST_ENGINE_INFO(sizeof(DWORD));
        ST_ENGINE_INFO("{0}", CPUInfo::GetCPUType());
        ST_ENGINE_INFO("{0}", CPUInfo::GetL1CacheLineSize());

        m_Window = Window::Create(props);

        m_Renderer = new TestRenderer();
        m_Renderer->Setup();
    }

    Application::~Application() {
        m_LayerStack.CleanLayerstack();
        UnsubscribeFromEvent(EventType::WindowClose, m_WindowCloseCallbackIndex);
        UnsubscribeFromEvent(EventType::WindowResize, m_WindowResizeCallbackIndex);
        UnsubscribeFromEvent(EventType::KeyPressed, m_KeyPressedCallbackIndex);
    }

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay) {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    const UInt32 Application::SubscribeToEvent(const EventType& eventType, const EventBus::EventCallbackFn& callback) {
        return m_EventBus.SubscribeToEvent(eventType, STL::move(callback));
    }

    void Application::UnsubscribeFromEvent(const EventType& eventType, const UInt32& callbackIndex) {
        m_EventBus.UnsubscribeFromEvent(eventType, callbackIndex);
    }

    void Application::Run() {
        while (m_Running) {
            if (!m_Minimized) {
                /* m_Renderer->PreRender();

                 m_Camera->OnUpdate();
                 ProcessLayerUpdate();

                 m_Renderer->FramebufferBind();
                 m_Renderer->DrawCommand();
                 m_Renderer->ClearCommand();
                 ProcessLayerImGuiRender();
                 m_Renderer->FramebufferUnbind();

                 m_Renderer->PostRender();*/
                m_Renderer->GetCamera()->OnUpdate();
                ProcessLayerUpdate();

                m_Renderer->Draw();
                ProcessLayerImGuiRender();
            }
            m_Window->OnUpdate();
            Input::OnUpdate();
        }

        // m_Renderer->InitShutdown();
    }

    void Application::RaiseEvent(UniqueRef<Event> eventData) {
        m_EventBus.NotifyAboutEvent(STL::move(eventData));
        m_EventBus.ProcessEvents();
    }

    void Application::ProcessLayerUpdate() {
        if (m_LayerStack.GetSize() == 0) return;

        for (Layer* layer: m_LayerStack) layer->OnUpdate();
    }

    void Application::ProcessLayerImGuiRender() {
        if (m_LayerStack.GetSize() == 0) return;

        m_ImGuiLayer->Begin();

        for (Layer* layer: m_LayerStack) layer->OnImGuiRender();

        m_ImGuiLayer->End();
    }

    void Application::OnWindowClose(Event& event) {
        WindowCloseEvent e = *(event.Cast<WindowCloseEvent>());
        m_Running = false;
        event.Handled = true;
    }

    void Application::OnWindowResize(Event& event) {
        WindowResizeEvent e = *(event.Cast<WindowResizeEvent>());
        // m_Camera->OnResize(e.GetWidth(), e.GetHeight());
        // m_Renderer->Resize(e.GetWidth(), e.GetHeight());
    }

    void Application::OnKeyPressed(Event& event) {
        KeyPressedEvent e = *(event.Cast<KeyPressedEvent>());
    }
}  // namespace Sentinel
