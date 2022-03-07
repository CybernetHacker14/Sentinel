#pragma once

#include "Sentinel/Window/Window.h"

struct GLFWwindow;

namespace Sentinel
{
	class WindowsWindow final : public WindowCRTP<WindowsWindow> {
	public:
		WindowsWindow(const WindowProperties& props);
		~WindowsWindow();

	private:
		void OnUpdate();

		UInt32 GetWidth() const { return m_Data.Width; }
		UInt32 GetHeight() const { return m_Data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		void SetVSync(Bool enabled);
		Bool IsVSync() const;

		void* GetNativeWindow() const { return m_Window; }

		void Init(const WindowProperties& props);
		void Shutdown();

	private:
		friend class WindowCRTP<WindowsWindow>;

		GLFWwindow* m_Window;

		struct WindowData {
			STL::string Title;
			UInt32 Width, Height;
			Bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
