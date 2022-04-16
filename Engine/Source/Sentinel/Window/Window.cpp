#include "stpch.h"
#include "Sentinel/Window/Window.h"

#ifdef ST_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif // ST_PLATFORM_WINDOWS

namespace Sentinel
{
	Window::Window(const WindowProperties& props)
		:m_Properties(props) {}

	UniqueRef<Window> Window::Create(const WindowProperties& props) {
	#ifdef ST_PLATFORM_WINDOWS
		return CreateUniqueRef<WindowsWindow>(props);
	#else
		ST_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif // ST_PLATFORM_WINDOWS
	}
}
