#include "stpch.h"
#include "Sentinel/Application/Application.h"
#include "Sentinel/Renderer/Core/Renderer.h"

namespace Sentinel
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const STL::string& name) {
		ST_ENGINE_ASSERT(!s_Instance, "Application instance already exist!");
		s_Instance = this;
		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(ST_BIND_EVENT_FN(Application::RaiseEvent));

		SubscribeToEvent(EventType::WindowClose, ST_BIND_EVENT_FN(Application::OnWindowClose));
		SubscribeToEvent(EventType::WindowResize, ST_BIND_EVENT_FN(Application::OnWindowResize));

		Renderer::Init();
	}

	Application::~Application() {
		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::SubscribeToEvent(const EventType& eventType, const EventBus::EventCallbackFn& callback) {
		m_EventBus.SubscribeToEvent(eventType, STL::move(callback));
	}

	void Application::Run() {
		ST_ENGINE_INFO("Starting Application Loop");
		while (m_Running)
		{
			m_EventBus.ProcessEventsDeferred();

			if (!m_Minimized)
			{
				// Update logic
				ProcessLayerUpdate();

				// Render stuff
				m_Window->OnUpdate();
			}
		}
	}

	void Application::RaiseEvent(Scope<Event> eventData) {
		m_EventBus.NotifyAboutEvent(STL::move(eventData));
		m_EventBus.ProcessEventsImmediate();
	}

	void Application::ProcessLayerUpdate() {
		// If there are no layers, then no need to execute extra for loops
		if (m_LayerStack.GetSize() == 0)
		{
			return;
		}

		for (auto layerStackIterator = m_LayerStack.rbegin();
			layerStackIterator != m_LayerStack.rend(); ++layerStackIterator)
		{
			(*layerStackIterator)->OnUpdate();
		}
	}

	void Application::OnWindowClose(Event& event) {
		WindowCloseEvent e = static_cast<WindowCloseEvent&>(event);
		m_Running = false;
		event.Handled = true;
	}

	void Application::OnWindowResize(Event& event) {
		WindowResizeEvent e = static_cast<WindowResizeEvent&>(event);
		ST_ENGINE_INFO("{0} {1}", e.GetWidth(), e.GetHeight());
		event.Handled = true;
	}
}
