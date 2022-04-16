#include "stpch.h"
#include "Sentinel/Application/Application.h"
#include "Sentinel/Events/Categories/WindowEvent.h"
#include "Sentinel/Events/Categories/KeyEvent.h"
#include "Sentinel/Input/Input.h"
#include "Sentinel/Input/KeyCodes.h"

#include "Sentinel/Math/Math.h"

#include "Sentinel/Graphics/Definitions/FrameBackings.h"
#include "Sentinel/Graphics/Definitions/RenderResources.h"

#include <imgui.h>

namespace Sentinel
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const STL::string& name) {
		ST_ENGINE_ASSERT(!s_Instance, "Application instance already exist!");
		s_Instance = this;

		m_Renderer = Renderer::Create();

		// Set frame backings definitions

		WindowProperties props;
		props.Title = name;
		props.Width = 1280;
		props.Height = 720;
		props.Mode = WindowMode::WINDOWED;
		props.FramebufferTransparency = false;

		FramebufferSpecification spec;
		spec.Attachments = { TextureFormat::RGBA32F };
		spec.ClearColor = { 0.1f, 0.5f, 0.1f, 0.1f };
		spec.Width = props.Width;
		spec.Height = props.Height;
		spec.SwapchainTarget = true;

		m_Renderer->SetRenderSpecifications(props, spec);

		// Set render resource data (geometry, textures, shaders, etc.)

		SharedRef<RenderResources> renderResource = RenderResources::Create();
		STL::vector<STL::pair<glm::vec4, glm::vec2>> vertices =
		{
			{ { -3.0f,   3.0f, -7.0f, 1.0f }, {0.0f, 0.0f} },
			{ { -1.0f,   3.0f, -7.0f, 1.0f }, {1.0f, 0.0f} },
			{ { -1.0f,   1.0f, -7.0f, 1.0f }, {1.0f, 1.0f} },
			{ { -3.0f,   1.0f, -7.0f, 1.0f }, {0.0f, 1.0f} },

			{ {  1.0f,   1.0f, -5.0f, 1.0f }, {0.0f, 0.0f} },
			{ {  3.0f,   1.0f, -5.0f, 1.0f }, {1.0f, 0.0f} },
			{ {  3.0f,  -1.0f, -5.0f, 1.0f }, {1.0f, 1.0f} },
			{ {  1.0f,  -1.0f, -5.0f, 1.0f }, {0.0f, 1.0f} }
		};

		renderResource->Vertexbuffers.emplace_back(
			Vertexbuffer::Create(vertices.data(),
				vertices.size() * sizeof(STL::pair<glm::vec4, glm::vec2>))
		);

		STL::vector<UInt32> indices =
		{
			0,1,2,0,2,3,
			4,5,6,4,6,7
		};

		renderResource->Indexbuffer = Indexbuffer::Create(indices.data(), indices.size());
		renderResource->Shader = Shader::Create("../Engine/Resources/Shaders/TextureShader.hlsl", "TextureShader");
		//renderResource->Shader = Shader::Create("TextureShader.hlsl", "TextureShader");

		Texture2DImportSettings settings;
		settings.texturePath = "Assets/Tile1.jpg";
		renderResource->Textures[0] = TextureTuple::Create();
		renderResource->Textures[0]->Texture = Texture2D::Create(settings);
		renderResource->Textures[0]->ShaderType = ShaderType::PIXEL;

		m_Renderer->SubmitGeometryData(renderResource);

		m_Renderer->GetWindow().SetEventCallback(ST_BIND_EVENT_FN(Application::RaiseEvent));

		m_WindowCloseCallbackIndex = SubscribeToEvent(EventType::WindowClose,
			ST_BIND_EVENT_FN(Application::OnWindowClose));
		m_WindowResizeCallbackIndex = SubscribeToEvent(EventType::WindowResize,
			ST_BIND_EVENT_FN(Application::OnWindowResize));
		m_KeyPressedCallbackIndex = SubscribeToEvent(EventType::KeyPressed,
			ST_BIND_EVENT_FN(Application::OnKeyPressed));

		m_Camera = Camera::Create(props.Width, props.Height);

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_ImGuiDebugLayer = new ImGuiDebugLayer(m_Camera);
		PushOverlay(m_ImGuiDebugLayer);

		m_Renderer->InitStartup();
	}

	Application::~Application() {
		m_LayerStack.CleanLayerstack();
		m_Renderer->Shutdown();
		UnsubscribeFromEvent(EventType::WindowClose, m_WindowCloseCallbackIndex);
		UnsubscribeFromEvent(EventType::WindowResize, m_WindowResizeCallbackIndex);
		UnsubscribeFromEvent(EventType::KeyPressed, m_KeyPressedCallbackIndex);
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
				m_Renderer->PreRender();

				m_Camera->OnUpdate();
				ProcessLayerUpdate();

				m_Renderer->FramebufferBind();
				m_Renderer->DrawCommand();
				m_Renderer->ClearCommand();
				ProcessLayerImGuiRender();
				m_Renderer->FramebufferUnbind();

				m_Renderer->PostRender();
			}
			m_Renderer->GetWindow().OnUpdate();
			Input::OnUpdate();
		}

		m_Renderer->InitShutdown();
	}

	void Application::RaiseEvent(UniqueRef<Event> eventData) {
		m_EventBus.NotifyAboutEvent(STL::move(eventData));
		m_EventBus.ProcessEvents();
	}

	void Application::ProcessLayerUpdate() {
		if (m_LayerStack.GetSize() == 0)
			return;

		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();
	}

	void Application::ProcessLayerImGuiRender() {
		if (m_LayerStack.GetSize() == 0)
			return;

		m_ImGuiLayer->Begin();

		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();

		m_ImGuiLayer->End();
	}

	void Application::OnWindowClose(Event& event) {
		WindowCloseEvent e = *(event.Cast<WindowCloseEvent>());
		m_Running = false;
		event.Handled = true;
	}

	void Application::OnWindowResize(Event& event) {
		WindowResizeEvent e = *(event.Cast<WindowResizeEvent>());
		m_Camera->OnResize(e.GetWidth(), e.GetHeight());
		m_Renderer->Resize(e.GetWidth(), e.GetHeight());
	}

	void Application::OnKeyPressed(Event& event) {
		KeyPressedEvent e = *(event.Cast<KeyPressedEvent>());
	}
}
