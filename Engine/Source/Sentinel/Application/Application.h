#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Events/EventBus.h"
#include "Sentinel/Layers/LayerStack.h"
#include "Sentinel/Graphics/Renderers/Renderer.h"
#include "Sentinel/AssetManagement/AssetManager.h"

#include "Sentinel/Graphics/Components/Buffers/Constantbuffer.h"
#include "Sentinel/Graphics/Components/Cameras/Camera.h"

#include "Sentinel/GUI/ImGui/ImGuiLayer.h"
#include "Sentinel/GUI/ImGui/ImGuiDebugLayer.h"

int main(int argc, char** argv);

namespace Sentinel
{
	class Application {
	public:
		Application(const STL::string& name = "Sentinel Engine");
		virtual ~Application();

		Window& GetWindow();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		const UInt32 SubscribeToEvent(const EventType& eventType, const EventBus::EventCallbackFn& callback);
		void UnsubscribeFromEvent(const EventType& eventType, const UInt32& callback);

		// Returns the Instance of the Application, since it's a singleton
		static Application& Get() { return *s_Instance; }
	private:
		// The main application loop
		void Run();

		void RaiseEvent(UniqueRef<Event> eventData);

		void ProcessLayerUpdate();

	private:
		void OnWindowClose(Event& event);
		void OnWindowResize(Event& event);
		void OnKeyPressed(Event& event);

	private:
		Bool m_Running = true;
		Bool m_Minimized = false;

		UInt32 m_WindowResizeCallbackIndex = 0;
		UInt32 m_WindowCloseCallbackIndex = 0;
		UInt32 m_KeyPressedCallbackIndex = 0;
	private:
		UniqueRef<AssetManager> m_AssetManager;
		UniqueRef<Renderer> m_Renderer;

		SharedRef<Texture2D> m_TileTexture;

		SharedRef<Constantbuffer> m_CameraCB;
		SharedRef<Camera> m_Camera;

	private:
		LayerStack m_LayerStack;
		EventBus m_EventBus;
		ImGuiLayer* m_ImGuiLayer;
		ImGuiDebugLayer* m_ImGuiDebugLayer;
	private:
		static Application* s_Instance;
		friend int::main(int argc, char** argv);
	};

	// TO BE Defined in Client
	Application* CreateApplication();
}
