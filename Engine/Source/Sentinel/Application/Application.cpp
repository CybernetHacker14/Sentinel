#include "stpch.h"
#include "Sentinel/Application/Application.h"
#include "Sentinel/Events/Categories/WindowEvent.h"
#include "Sentinel/Input/Input.h"

#include "Sentinel/Math/Math.h"

#include "Sentinel/Graphics/Components/Buffers/Constantbuffer.h"
#include "Platform/DirectX11/Graphics/Components/Buffers/DX11Constantbuffer.h"

namespace Sentinel
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const STL::string& name) {
		ST_ENGINE_ASSERT(!s_Instance, "Application instance already exist!");
		s_Instance = this;

		SharedRef<DeviceModules> deviceModules = CreateSharedRef<DeviceModules>();
		deviceModules->WindowProperties = CreateUniqueRef<WindowProperties>(name, 1280, 720, WindowMode::WINDOWED, false);

		m_Renderer = UniqueRef<Renderer>(CreateUniqueRef<Renderer>(deviceModules));
		m_Renderer->GetWindow().SetEventCallback(ST_BIND_EVENT_FN(Application::RaiseEvent));

		m_WindowCloseCallbackIndex = SubscribeToEvent(EventType::WindowClose, ST_BIND_EVENT_FN(Application::OnWindowClose));
		m_WindowResizeCallbackIndex = SubscribeToEvent(EventType::WindowResize, ST_BIND_EVENT_FN(Application::OnWindowResize));

		SharedRef<PipelineModules> pipelineModules = CreateSharedRef<PipelineModules>();

		STL::vector<STL::pair<glm::vec4, glm::vec2>> vertices =
		{
			{ { -1.0f,   1.0f, -0.3f, 1.0f }, {0.0f, 0.0f} },
			{ {  1.0f,   1.0f, -0.3f, 1.0f }, {1.0f, 0.0f} },
			{ {  1.0f,  -1.0f, -0.3f, 1.0f }, {1.0f, 1.0f} },
			{ { -1.0f,  -1.0f, -0.3f, 1.0f }, {0.0f, 1.0f} }
		};

		SharedRef<Vertexbuffer> vertexBuffer = Vertexbuffer::Create(vertices.data(),
			vertices.size() * sizeof(STL::pair<glm::vec4, glm::vec4>));

		STL::vector<UInt32> indices =
		{
			0,1,2,0,2,3
		};

		FramebufferSpecification spec;
		spec.Attachments = { TextureFormat::RGBA32F };
		spec.ClearColor = { 0.1f, 0.1f, 0.1f, 0.1f };
		spec.Width = deviceModules->WindowProperties->Width;
		spec.Height = deviceModules->WindowProperties->Height;
		spec.SwapchainTarget = true;

		Texture2DImportSettings settings;
		settings.texturePath = "Assets/Tile1.jpg";
		m_TileTexture = Texture2D::Create(settings);

		pipelineModules->Framebuffer = Framebuffer::Create(spec);
		pipelineModules->Vertexbuffers.emplace_back(vertexBuffer);
		pipelineModules->Indexbuffer = Indexbuffer::Create(indices.data(), indices.size());
		pipelineModules->Shader = Shader::Create(/*"../Engine/Resources/Shaders/*/"TextureShader.hlsl", "TextureShader");
		m_Renderer->SetPipelineData(pipelineModules);

		m_TileTexture->Bind(0, ShaderType::PIXEL);

		m_Camera = CreateUniqueRef<Camera>(deviceModules->WindowProperties->Width, deviceModules->WindowProperties->Height);
		m_CameraCB = Constantbuffer::Create(sizeof(glm::mat4), 0, Constantbuffer::UsageType::DYNAMIC);

		m_CameraCB->SetStaticData(&(m_Camera->GetViewProjectionMatrix()));
		m_CameraCB->VSBind();
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
				m_CameraCB->SetDynamicData(&(m_Camera->GetViewProjectionMatrix()));
				m_CameraCB->VSBind();
				m_Renderer->Draw();
			}
			m_Renderer->GetWindow().OnUpdate();
			Input::OnUpdate();
		}
	}

	void Application::RaiseEvent(UniqueRef<Event> eventData) {
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
		m_Camera->OnResize(e.GetWidth(), e.GetHeight());
	}
}
