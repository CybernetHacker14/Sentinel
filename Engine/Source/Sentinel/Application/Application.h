#pragma once

#include "stpch.h"

#include "Sentinel/Base/Define.h"
#include "Sentinel/Events/EventBus.h"
#include "Sentinel/Events/Categories/WindowEvent.h"
#include "Sentinel/Layers/LayerStack.h"
#include "Sentinel/Window/Window.h"

int main(int argc, char** argv);

namespace Sentinel
{
	class Application {
	public:
		Application(const STL::string& name = "Sentinel Engine");
		virtual ~Application();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		// Returns the Instance of the Application, since it's a singleton
		static Application& Get() { return *s_Instance; }
	private:
		// The main application loop
		void Run();

		void RaiseEvent(Scope<Event> eventData);

		// All events will be sent to each layer one by one in order to be evaluated
		void ProcessEventData();

		// Each layer's OnUpdate() function will be called one by one
		void ProcessLayerUpdate();

		void OnWindowClose(Event& event);
		void OnWindowResize(Event& event);

	private:
		bool m_Running = true;
		bool m_Minimized = false;
	private:
		Scope<Window> m_Window;
		LayerStack m_LayerStack;
		EventBus m_EventBus;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// TO BE Defined in Client
	Application* CreateApplication();
}
