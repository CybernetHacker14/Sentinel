#pragma once

#include "Sentinel/Graphics/Modules/GraphicsContext.h"

// Forward declaration
struct GLFWwindow;

namespace Sentinel
{
	class DX11GraphicsContext final : public GraphicsContext<DX11GraphicsContext> {
	public:
		DX11GraphicsContext(GLFWwindow* windowHandle);

		void Init();

	private:
		void SetContextInfo();

	private:
		GLFWwindow* m_WindowHandle;
	};
}
