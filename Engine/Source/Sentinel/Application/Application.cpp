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

		Renderer::Init();
	}

	Application::Application(const char* name) {
		ST_ENGINE_ASSERT(!s_Instance, "Application instance already exist!");
		s_Instance = this;
		m_Window = Window::Create(WindowProps(name));

		Renderer::Init();
	}

	Application::~Application() {
		Renderer::Shutdown();
	}

	void Application::OnEvent(Event& event) {
		m_EventBus.PushEvent(&event);
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
			// Process input
			ProcessEventData();

			if (!m_Minimized)
			{
				// Update logic
				ProcessLayerUpdate();

				// Render stuff
				m_Window->OnUpdate();
			}
		}
	}

	void Application::ProcessEventData() {
		// If there are no events or layers, then no need to execute extra for loops
		if (m_EventBus.GetSize() == 0 || m_LayerStack.GetSize() == 0)
		{
			return;
		}

		for (auto layerStackIterator = m_LayerStack.rbegin();
			layerStackIterator != m_LayerStack.rend(); ++layerStackIterator)
		{
			for (auto eventBusIterator = m_EventBus.begin();
				eventBusIterator != m_EventBus.end(); ++eventBusIterator)
			{
				(*layerStackIterator)->OnEvent(**eventBusIterator);
				if ((**eventBusIterator).Handled)
				{
					m_EventBus.PopEvent(*eventBusIterator);
				}
			}
		}
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
}
