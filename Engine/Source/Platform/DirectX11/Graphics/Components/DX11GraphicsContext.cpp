#include "stpch.h"

#include "Platform/DirectX11/Graphics/Components/DX11GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Sentinel
{
	DX11GraphicsContext::DX11GraphicsContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle) {
		ST_ENGINE_ASSERT(m_WindowHandle, "Window Handle is null");
	}

	void DX11GraphicsContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);

		SetContextInfo();
	}

	void DX11GraphicsContext::SetContextInfo() {
		ST_ENGINE_INFO("Getting here");
	}
}
