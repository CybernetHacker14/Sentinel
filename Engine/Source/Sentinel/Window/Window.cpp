#include "stpch.h"
#include "Sentinel/Window/Window.h"
#include "Sentinel/Application/Application.h"

#ifdef ST_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif // ST_PLATFORM_WINDOWS

namespace Sentinel
{
	Scope<Window> Window::Create(const WindowProps& props) {
	#ifdef ST_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
	#else
		ST_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif // ST_PLATFORM_WINDOWS
	}
}
