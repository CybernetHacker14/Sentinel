#pragma once

#include "Sentinel/Window/Window.h"

struct GLFWwindow;

namespace Sentinel
{
	class WindowsWindow final : public WindowCRTP<WindowsWindow> {
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

	private:
		void OnUpdate();

		UInt GetWidth() const { return m_Data.Width; }
		UInt GetHeight() const { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		void SetVSync(Bool enabled);
		Bool IsVSync() const;

		void* GetNativeWindow() const { return m_Window; }

		void Init(const WindowProps& props);
		void Shutdown();

	private:
		friend class WindowCRTP<WindowsWindow>;

		GLFWwindow* m_Window;

		struct WindowData {
			STL::string Title;
			UInt Width, Height;
			Bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
