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

		m_EventBus.SubscribeToEvent("Sentinel::WindowCloseEvent", ST_BIND_EVENT_FN(Application::OnWindowClose));
		m_EventBus.SubscribeToEvent("Sentinel::WindowResizeEvent", ST_BIND_EVENT_FN(Application::OnWindowResize));

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

	void Application::Run() {
		ST_ENGINE_INFO("Starting Application Loop");
		while (m_Running)
		{
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
		m_EventBus.RegisterEvent(STL::move(eventData));
		m_EventBus.ProcessEventsImmediate();
	}

	void Application::ProcessEventData() {
		// If there are no events or layers, then no need to execute extra for loops
		/*if (m_EventBus.GetSize() == 0 || m_LayerStack.GetSize() == 0)
		{
			return;
		}*/

		//m_EventBus.ProcessEvents();

		//for (auto layerStackIterator = m_LayerStack.rbegin();
		//	layerStackIterator != m_LayerStack.rend(); ++layerStackIterator)
		//{
		//	/*for (auto eventBusIterator = m_EventBus.begin();
		//		eventBusIterator != m_EventBus.end(); ++eventBusIterator)
		//	{
		//		OnEvent(*eventBusIterator);
		//		(*layerStackIterator)->OnEvent(*eventBusIterator);
		//		if ((**eventBusIterator).Handled)
		//		{
		//			m_EventBus.PopEvent(*eventBusIterator);
		//		}
		//	}*/
		//}
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
		event.Handled = true;
		m_Running = false;
	}
	void Application::OnWindowResize(Event& event) {
		WindowResizeEvent e = static_cast<WindowResizeEvent&>(event);
		ST_ENGINE_INFO("{0} {1}", e.GetWidth(), e.GetHeight());
		event.Handled = true;
	}
}
