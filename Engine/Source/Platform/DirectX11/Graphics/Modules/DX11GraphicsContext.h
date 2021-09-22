#pragma once

#include "Sentinel/Graphics/Modules/GraphicsContext.h"

struct GLFWwindow;

namespace Sentinel
{
	class DX11GraphicsContext final : public GraphicsContextCRTP<DX11GraphicsContext> {
	public:
		DX11GraphicsContext(GLFWwindow* windowHandle);

	private:
		void Init();

		void InitializeDeviceAndSwapchain();
		void InitializeDirectXComponents();

		void SetContextInfo();

	private:
		friend class GraphicsContextCRTP<DX11GraphicsContext>;

		GLFWwindow* m_WindowHandle;
	};
}
