#pragma once

#include "Sentinel/Events/Categories/WindowEvent.h"

namespace Sentinel
{
	struct WindowProps {
		STL::string Title;
		UInt Width;
		UInt Height;
		Bool FullScreen;
		Bool WindowedMaximized;

		WindowProps(
			const STL::string& title = "Sentinel-Engine",
			UInt width = 900,
			UInt height = 900,
			Bool fullscreen = false,
			Bool windowedMaximized = false)
			:Title(title), Width(width), Height(height),
			FullScreen(fullscreen), WindowedMaximized(windowedMaximized) {}
	};

	// Interface representing a desktop system based Window
	class Window {
	public:

		using EventCallbackFn = STL::function<void(Scope<Event>)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual UInt GetWidth() const = 0;
		virtual UInt GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(Bool enabled) = 0;
		virtual Bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}
