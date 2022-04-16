#pragma once

#include "Sentinel/Graphics/Components/FrameBackings/Misc/GraphicsContext.h"

struct GLFWwindow;

namespace Sentinel
{
	class Window;

	class DX11GraphicsContext final : public GraphicsContext {
	public:
		DX11GraphicsContext(GLFWwindow* windowHandle);

	private:
		void Init();

		void InitializeDeviceAndSwapchain();
		void InitializeDirectXComponents();

		void SetContextInfo();

	private:
		GLFWwindow* m_WindowHandle;
	};
}
