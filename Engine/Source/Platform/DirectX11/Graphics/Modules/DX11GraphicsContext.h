#pragma once

#include "Sentinel/Graphics/Modules/GraphicsContext.h"

struct GLFWwindow;

namespace Sentinel
{
	class DX11GraphicsContext final : public GraphicsContextCRTP<DX11GraphicsContext> {
	public:
		DX11GraphicsContext(GLFWwindow* windowHandle);

		void Init();

	private:
		void InitializeDeviceAndSwapchain();
		void InitializeDirectXComponents();

		void SetContextInfo();

	private:
		GLFWwindow* m_WindowHandle;
	};
}
