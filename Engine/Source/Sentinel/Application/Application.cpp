#include "stpch.h"

#include "Sentinel/Application/Application.h"
#include "Sentinel/Events/Categories/WindowEvent.h"
#include "Sentinel/Input/Input.h"

namespace Sentinel
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const STL::string& name) {
		ST_ENGINE_ASSERT(!s_Instance, "Application instance already exist!");
		s_Instance = this;

		Ref<DeviceModules> deviceModules = CreateRef<DeviceModules>();
		deviceModules->WindowProps = CreateScope<WindowProps>(name);

		m_Renderer = CreateScope<Renderer>(deviceModules);
		m_Renderer->GetWindow().SetEventCallback(ST_BIND_EVENT_FN(Application::RaiseEvent));

		m_WindowCloseCallbackIndex = SubscribeToEvent(EventType::WindowClose, ST_BIND_EVENT_FN(Application::OnWindowClose));
		m_WindowResizeCallbackIndex = SubscribeToEvent(EventType::WindowResize, ST_BIND_EVENT_FN(Application::OnWindowResize));

		Ref<PipelineModules> pipelineModules = CreateRef<PipelineModules>();
		pipelineModules->ClearColor = { 0.0f, 0.2f, 0.3f, 1.0f };


		STL::vector<STL::pair<glm::vec4, glm::vec4>> vertices =
		{
			{ { -0.5f,   0.2f, 0.0f, 1.0f }, {1.0f, 0.0f, 0.0f, 1.0f} },
			{ { -0.25f, -0.4f, 0.0f, 1.0f }, {0.0f, 1.0f, 0.0f, 1.0f} },
			{ { -0.75f, -0.4f, 0.0f, 1.0f }, {0.0f, 0.0f, 1.0f, 1.0f} },
			{ {   0.5f,  0.2f, 0.0f, 1.0f }, {0.5f, 1.0f, 0.0f, 1.0f} },
			{ {  0.75f, -0.4f, 0.0f, 1.0f }, {1.0f, 0.0f, 0.5f, 1.0f} },
			{ {  0.25f, -0.4f, 0.0f, 1.0f }, {0.0f, 0.5f, 1.0f, 1.0f} }
		};

		Ref<Vertexbuffer> vertexBuffer = Vertexbuffer::Create(vertices.data(),
			vertices.size() * sizeof(STL::pair<glm::vec4, glm::vec4>));

		STL::vector<UInt32> indices =
		{
			0,1,2,3,4,5
		};

		Ref<Indexbuffer> indexBuffer = Indexbuffer::Create(indices.data(), indices.size());

		pipelineModules->Vertexbuffers.emplace_back(vertexBuffer);
		pipelineModules->Indexbuffer = indexBuffer;
		pipelineModules->Shader = Shader::Create("../Engine/Resources/Shaders/TestShader.hlsl", "TestShader");
		m_Renderer->SetPipelineData(pipelineModules);

		m_AssetManager = CreateScope<AssetManager>();
	}

	Application::~Application() {
		m_Renderer->Shutdown();
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

	const UInt32 Application::SubscribeToEvent(const EventType& eventType, const EventBus::EventCallbackFn& callback) {
		return m_EventBus.SubscribeToEvent(eventType, STL::move(callback));
	}

	void Application::UnsubscribeFromEvent(const EventType& eventType, const UInt32& callbackIndex) {
		m_EventBus.UnsubscribeFromEvent(eventType, callbackIndex);
	}

	void Application::Run() {
		while (m_Running)
		{
			if (!m_Minimized)
			{
				ProcessLayerUpdate();

				m_Renderer->Draw();
			}
			m_Renderer->GetWindow().OnUpdate();
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
		WindowCloseEvent e = *(event.DerivedDowncast<WindowCloseEvent>());
		m_Running = false;
		event.Handled = true;
	}

	void Application::OnWindowResize(Event& event) {
		WindowResizeEvent e = *(event.DerivedDowncast<WindowResizeEvent>());
	}
}
