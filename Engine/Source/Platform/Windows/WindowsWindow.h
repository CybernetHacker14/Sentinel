#pragma once

#include "Sentinel/Window/Window.h"
#include "Sentinel/Graphics/Components/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Sentinel
{
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		UInt GetWidth() const override { return m_Data.Width; }
		UInt GetHeight() const override { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(Bool enabled) override;
		Bool IsVSync() const override;

		virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContextBase> m_Context;

		struct WindowData {
			STL::string Title;
			UInt Width, Height;
			Bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
