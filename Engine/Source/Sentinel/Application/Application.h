#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Events/EventBus.h"
#include "Sentinel/Layers/LayerStack.h"
#include "Sentinel/Graphics/Renderers/Renderer.h"
#include "Sentinel/AssetManagement/AssetManager.h"

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

	private:
		Bool m_Running = true;
		Bool m_Minimized = false;

		UInt32 m_WindowResizeCallbackIndex = 0;
		UInt32 m_WindowCloseCallbackIndex = 0;
	private:
		UniqueRef<AssetManager> m_AssetManager;
		UniqueRef<Renderer> m_Renderer;

		SharedRef<Texture2D> m_TileTexture;

	private:
		LayerStack m_LayerStack;
		EventBus m_EventBus;
	private:
		static Application* s_Instance;
		friend int::main(int argc, char** argv);
	};

	// TO BE Defined in Client
	Application* CreateApplication();
}
