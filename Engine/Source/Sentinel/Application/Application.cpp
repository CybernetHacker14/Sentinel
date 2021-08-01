#include "stpch.h"

#include "Sentinel/Application/Application.h"
#include "Sentinel/Input/Input.h"
#include "Sentinel/Renderer/Core/Renderer.h"

#include <glad/glad.h>

namespace Sentinel
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const STL::string& name) {
		ST_ENGINE_ASSERT(!s_Instance, "Application instance already exist!");
		s_Instance = this;
		m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(ST_BIND_EVENT_FN(Application::RaiseEvent));

		m_WindowCloseCallbackIndex = SubscribeToEvent(EventType::WindowClose, ST_BIND_EVENT_FN(Application::OnWindowClose));

		/*m_WindowCloseCallbackIndex = SubscribeToEvent(EventType::WindowClose, ST_BIND_EVENT_FN(Application::OnWindowClose));
		m_WindowResizeCallbackIndex = SubscribeToEvent(EventType::WindowResize, ST_BIND_EVENT_FN(Application::OnWindowResize));
		m_KeyPressedCallbackIndex = SubscribeToEvent(EventType::KeyPressed, ST_BIND_EVENT_FN(Application::OnKeyPressed));
		m_KeyReleasedCallbackIndex = SubscribeToEvent(EventType::KeyReleased, ST_BIND_EVENT_FN(Application::OnKeyReleased));
		m_KeyTypedCallbackIndex = SubscribeToEvent(EventType::KeyTyped, ST_BIND_EVENT_FN(Application::OnKeyTyped));
		m_MouseButtonPressedCallbackIndex =
			SubscribeToEvent(EventType::MouseButtonPressed, ST_BIND_EVENT_FN(Application::OnMouseButtonPressed));
		m_MouseButtonReleasedCllbackIndex =
			SubscribeToEvent(EventType::MouseButtonReleased, ST_BIND_EVENT_FN(Application::OnMouseButtonReleased));
		m_MouseButtonScrollCallbackIndex =
			SubscribeToEvent(EventType::MouseScrolled, ST_BIND_EVENT_FN(Application::OnMouseButtonScrolled));
		m_MouseMovedCallbackIndex =
			SubscribeToEvent(EventType::MouseMoved, ST_BIND_EVENT_FN(Application::OnMouseMoved));*/

		Renderer::Init();
		RenderCommand::SetClearColor({ 0.0f, 0.2f, 0.4f, 1.0f });

		/*glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.0f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/
	}

	Application::~Application() {
		UnsubscribeFromEvent(EventType::WindowClose, m_WindowCloseCallbackIndex);

		/*UnsubscribeFromEvent(EventType::WindowClose, m_WindowCloseCallbackIndex);
		UnsubscribeFromEvent(EventType::WindowResize, m_WindowResizeCallbackIndex);
		UnsubscribeFromEvent(EventType::KeyPressed, m_KeyPressedCallbackIndex);
		UnsubscribeFromEvent(EventType::KeyReleased, m_KeyReleasedCallbackIndex);
		UnsubscribeFromEvent(EventType::KeyTyped, m_KeyTypedCallbackIndex);
		UnsubscribeFromEvent(EventType::MouseButtonPressed, m_MouseButtonPressedCallbackIndex);
		UnsubscribeFromEvent(EventType::MouseButtonReleased, m_MouseButtonReleasedCllbackIndex);
		UnsubscribeFromEvent(EventType::MouseScrolled, m_MouseButtonScrollCallbackIndex);
		UnsubscribeFromEvent(EventType::MouseMoved, m_MouseMovedCallbackIndex);*/
		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	const uint32_t Application::SubscribeToEvent(const EventType& eventType, const EventBus::EventCallbackFn& callback) {
		return m_EventBus.SubscribeToEvent(eventType, STL::move(callback));
	}

	void Application::UnsubscribeFromEvent(const EventType& eventType, const uint32_t& callbackIndex) {
		m_EventBus.UnsubscribeFromEvent(eventType, callbackIndex);
	}

	void Application::Run() {
		while (m_Running)
		{
			if (!m_Minimized)
			{
				/*glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

				glBindVertexArray(m_VertexArray);
				glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);*/

				RenderCommand::Clear();

				ProcessLayerUpdate();

				m_Window->OnUpdate();
			}

			Input::OnUpdate();
		}
	}

	void Application::RaiseEvent(Scope<Event> eventData) {
		m_EventBus.NotifyAboutEvent(STL::move(eventData));
		m_EventBus.ProcessEvents();
	}

	void Application::ProcessLayerUpdate() {
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
		ST_ENGINE_INFO("{0}", e.ToString().c_str());
		event.Handled = true;
	}

	void Application::OnKeyPressed(Event& event) {
		KeyPressedEvent e = static_cast<KeyPressedEvent&>(event);
		ST_ENGINE_INFO("{0}", e.ToString().c_str());
		event.Handled = true;
	}

	void Application::OnKeyReleased(Event& event) {
		KeyReleasedEvent e = static_cast<KeyReleasedEvent&>(event);
		ST_ENGINE_INFO("{0}", e.ToString().c_str());
		event.Handled = true;
	}

	void Application::OnKeyTyped(Event& event) {
		KeyTypedEvent e = static_cast<KeyTypedEvent&>(event);
		ST_ENGINE_INFO("{0}", e.ToString().c_str());
		event.Handled = true;
	}

	void Application::OnMouseButtonPressed(Event& event) {
		MouseButtonPressedEvent e = static_cast<MouseButtonPressedEvent&>(event);
		ST_ENGINE_INFO("{0}", e.ToString().c_str());
		event.Handled = true;
	}

	void Application::OnMouseButtonReleased(Event& event) {
		MouseButtonReleasedEvent e = static_cast<MouseButtonReleasedEvent&>(event);
		ST_ENGINE_INFO("{0}", e.ToString().c_str());
		event.Handled = true;
	}

	void Application::OnMouseButtonScrolled(Event& event) {
		MouseScrolledEvent e = static_cast<MouseScrolledEvent&>(event);
		ST_ENGINE_INFO("{0}", e.ToString().c_str());
		event.Handled = true;
	}

	void Application::OnMouseMoved(Event& event) {
		MouseMovedEvent e = static_cast<MouseMovedEvent&>(event);
		ST_ENGINE_INFO("{0}", e.ToString().c_str());
		event.Handled = true;
	}
}
