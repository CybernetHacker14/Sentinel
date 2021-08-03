#pragma once

#include "Sentinel/Events/Categories/WindowEvent.h"

namespace Sentinel
{
	struct WindowProps {
		STL::string Title;
		uint32_t Width;
		uint32_t Height;
		bool FullScreen;
		bool WindowedMaximized;

		WindowProps(
			const STL::string& title = "Sentinel-Engine",
			uint32_t width = 900,
			uint32_t height = 900,
			bool fullscreen = false,
			bool windowedMaximized = false)
			:Title(title), Width(width), Height(height),
			FullScreen(fullscreen), WindowedMaximized(windowedMaximized) {}
	};

	// Interface representing a desktop system based Window
	class Window {
	public:

		using EventCallbackFn = STL::function<void(Scope<Event>)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}
