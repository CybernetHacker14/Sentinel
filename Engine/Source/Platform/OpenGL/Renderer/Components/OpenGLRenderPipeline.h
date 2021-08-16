#pragma once

#include "Sentinel/Renderer/Components/RenderPipeline.h"

struct GLFWwindow;

namespace Sentinel
{
	class OpenGLRenderPipeline : public RenderPipeline {
	public:
		OpenGLRenderPipeline(const Scope<Window>& window);
		virtual ~OpenGLRenderPipeline();

		virtual void InitPipeline() override;
		virtual void CleanPipeline() override;

		virtual void Draw() override;
	protected:
		virtual void InitGraphics() override;

	private:
		GLFWwindow* m_NativeWindow;
	};
}
