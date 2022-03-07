#include "stpch.h"
#include "Sentinel/Window/Window.h"

#ifdef ST_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif // ST_PLATFORM_WINDOWS

namespace Sentinel
{
	void Window::OnUpdate() {
	#ifdef ST_PLATFORM_WINDOWS
		BaseDowncast<WindowsWindow>()->OnUpdate();
	#else
		ST_ASSERT(false, "Unknown platform!");
	#endif // ST_PLATFORM_WINDOWS
	}

	UInt32 Window::GetWidth() {
	#ifdef ST_PLATFORM_WINDOWS
		return BaseDowncast<WindowsWindow>()->GetWidth();
	#else
		ST_ASSERT(false, "Unknown platform!");
		return UInt();
	#endif // ST_PLATFORM_WINDOWS
	}

	UInt32 Window::GetHeight() {
	#ifdef ST_PLATFORM_WINDOWS
		return BaseDowncast<WindowsWindow>()->GetHeight();
	#else
		ST_ASSERT(false, "Unknown platform!");
		return UInt();
	#endif // ST_PLATFORM_WINDOWS
	}

	void Window::SetEventCallback(const EventCallbackFn& callback) {
	#ifdef ST_PLATFORM_WINDOWS
		BaseDowncast<WindowsWindow>()->SetEventCallback(callback);
	#else
		ST_ASSERT(false, "Unknown platform!");
	#endif // ST_PLATFORM_WINDOWS
	}

	void Window::SetVSync(Bool enabled) {
	#ifdef ST_PLATFORM_WINDOWS
		BaseDowncast<WindowsWindow>()->SetVSync(enabled);
	#else
		ST_ASSERT(false, "Unknown platform!");
	#endif // ST_PLATFORM_WINDOWS
	}

	Bool Window::IsVSync() {
	#ifdef ST_PLATFORM_WINDOWS
		return BaseDowncast<WindowsWindow>()->IsVSync();
	#else
		ST_ASSERT(false, "Unknown platform!");
		return Bool();
	#endif // ST_PLATFORM_WINDOWS
	}

	void* Window::GetNativeWindow() {
	#ifdef ST_PLATFORM_WINDOWS
		return BaseDowncast<WindowsWindow>()->GetNativeWindow();
	#else
		ST_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif // ST_PLATFORM_WINDOWS
	}

	UniqueRef<Window> Window::Create(const WindowProperties& props) {
	#ifdef ST_PLATFORM_WINDOWS
		return CreateUniqueRef<WindowsWindow>(props);
	#else
		ST_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif // ST_PLATFORM_WINDOWS
	}
}
