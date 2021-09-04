#include "stpch.h"

#include "Sentinel/Application/Application.h"
#include "Sentinel/Input/Input.h"

namespace Sentinel
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const STL::string& name) {
		ST_ENGINE_ASSERT(!s_Instance, "Application instance already exist!");
		s_Instance = this;

		Ref<DeviceModules> deviceModules = CreateRef<DeviceModules>();
		deviceModules->WindowProps = CreateScope<WindowProps>(name);

		Ref<PipelineModules> pipelineModules = CreateRef<PipelineModules>();
		pipelineModules->ClearColor = { 0.0f, 0.2f, 0.4f, 1.0f };

		glm::vec3 vertices[3] =
		{
			{0.0f, 0.0f, 0.0f},
			{0.5f, -0.5f, 0.0f},
			{-0.5f, -0.5f, 0.0f}
		};

		Ref<VertexbufferBase> vertexBuffer = VertexbufferUtils::Create(vertices, 3 * 3 * sizeof(Float));

		vertexBuffer->BaseDowncast<DX11Vertexbuffer>()->SetLayout({
			{ShaderDataType::Float3, "position"}
			});

		pipelineModules->Vertexbuffers.emplace_back(vertexBuffer);

		m_Renderer = CreateScope<Renderer>(deviceModules, pipelineModules);
		m_Renderer->GetWindow().SetEventCallback(ST_BIND_EVENT_FN(Application::RaiseEvent));
		/*m_Window = Window::Create(WindowProps(name));
		m_Window->SetEventCallback(ST_BIND_EVENT_FN(Application::RaiseEvent));*/

		m_WindowCloseCallbackIndex = SubscribeToEvent(EventType::WindowClose, ST_BIND_EVENT_FN(Application::OnWindowClose));
		m_WindowResizeCallbackIndex = SubscribeToEvent(EventType::WindowResize, ST_BIND_EVENT_FN(Application::OnWindowResize));
	}

	Application::~Application() {
		UnsubscribeFromEvent(EventType::WindowClose, m_WindowCloseCallbackIndex);
		UnsubscribeFromEvent(EventType::WindowResize, m_WindowResizeCallbackIndex);
	}

	Window& Application::GetWindow() {
		return m_Renderer->GetWindow();
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	const UInt Application::SubscribeToEvent(const EventType& eventType, const EventBus::EventCallbackFn& callback) {
		return m_EventBus.SubscribeToEvent(eventType, STL::move(callback));
	}

	void Application::UnsubscribeFromEvent(const EventType& eventType, const UInt& callbackIndex) {
		m_EventBus.UnsubscribeFromEvent(eventType, callbackIndex);
	}

	void Application::Run() {
		while (m_Running)
		{
			if (!m_Minimized)
			{
				ProcessLayerUpdate();

				//m_Window->OnUpdate();
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
