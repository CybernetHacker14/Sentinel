#pragma once

#include "stpch.h"

#include "Sentinel/Base/Define.h"
#include "Sentinel/Events/EventBus.h"
#include "Sentinel/Events/Categories/WindowEvent.h"
#include "Sentinel/Events/Categories/KeyEvent.h"
#include "Sentinel/Events/Categories/MouseEvent.h"
#include "Sentinel/Layers/LayerStack.h"
#include "Sentinel/Window/Window.h"

int main(int argc, char** argv);

namespace Sentinel
{
	class Application {
	public:
		Application(const STL::string& name = "Sentinel Engine");
		virtual ~Application();

		Window& GetWindow() { return *m_Window; }

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		const uint32_t SubscribeToEvent(const EventType& eventType, const EventBus::EventCallbackFn& callback);
		void UnsubscribeFromEvent(const EventType& eventType, const uint32_t& callback);

		// Returns the Instance of the Application, since it's a singleton
		static Application& Get() { return *s_Instance; }
	private:
		// The main application loop
		void Run();

		void RaiseEvent(Scope<Event> eventData);

		void ProcessLayerUpdate();

	private:
		void OnWindowClose(Event& event);
		void OnWindowResize(Event& event);

		void OnKeyPressed(Event& event);
		void OnKeyReleased(Event& event);
		void OnKeyTyped(Event& event);

		void OnMouseButtonPressed(Event& event);
		void OnMouseButtonReleased(Event& event);
		void OnMouseButtonScrolled(Event& event);
		void OnMouseMoved(Event& event);

	private:
		bool m_Running = true;
		bool m_Minimized = false;

		uint32_t m_WindowResizeCallbackIndex = 0;
		uint32_t m_WindowCloseCallbackIndex = 0;
		uint32_t m_KeyPressedCallbackIndex = 0;
		uint32_t m_KeyReleasedCallbackIndex = 0;
		uint32_t m_KeyTypedCallbackIndex = 0;
		uint32_t m_MouseButtonPressedCallbackIndex = 0;
		uint32_t m_MouseButtonReleasedCllbackIndex = 0;
		uint32_t m_MouseButtonScrollCallbackIndex = 0;
		uint32_t m_MouseMovedCallbackIndex = 0;

		//uint32_t m_VertexArray, m_VertexBuffer, m_IndexBuffer;
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
