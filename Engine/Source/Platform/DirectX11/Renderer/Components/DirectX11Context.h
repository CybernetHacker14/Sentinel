#pragma once

#include "Sentinel/Renderer/Components/GraphicsContext.h"

// Forward declaration
struct GLFWwindow;

namespace Sentinel
{
	class DirectX11Context : public GraphicsContext {
	public:
		DirectX11Context(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
		ContextInfo m_ContextInfo;
	};
}
