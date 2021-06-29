#pragma once

#include "stpch.h"

#include "Sentinel/Base/Define.h"
#include "Sentinel/Events/EventBus.h"
#include "Sentinel/Layers/LayerStack.h"
#include "Sentinel/Window/Window.h"

int main(int argc, char** argv);

namespace Sentinel
{
	class Application {
	public:
		Application(const STL::string& name = "Sentinel Engine");
		Application(const char* name = "Sentinel Engine");
		virtual ~Application();

		// One of the core points of Event and Layer functionality.
		// A Window class will bind this function to callbacks, where the
		// event data will be sent via the parameter.
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		// Returns the Instance of the Application, since it's a singleton
		static Application& Get() { return *s_Instance; }
	private:
		// The main application loop
		void Run();

		// All events will be sent to each layer one by one in order to be evaluated
		void ProcessEventData();

		// Each layer's OnUpdate() function will be called one by one
		void ProcessLayerUpdate();
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
