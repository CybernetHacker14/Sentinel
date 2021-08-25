#pragma once

#include "Sentinel/Graphics/Components/GraphicsContext.h"

// Forward declaration
struct GLFWwindow;

namespace Sentinel
{
	class DX11GraphicsContext : public GraphicsContext<DX11GraphicsContext> {
	public:
		DX11GraphicsContext(GLFWwindow* windowHandle);

		void Init();

	private:
		void SetContextInfo();

	private:
		GLFWwindow* m_WindowHandle;
	};
}
