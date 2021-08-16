#include "stpch.h"

#include "Platform/OpenGL/Renderer/Components/OpenGLRenderPipeline.h"

namespace Sentinel
{
	OpenGLRenderPipeline::OpenGLRenderPipeline(const Scope<Window>& window) {
		m_NativeWindow = (GLFWwindow*)window->GetNativeWindow();
	}
	OpenGLRenderPipeline::~OpenGLRenderPipeline() {}
	void OpenGLRenderPipeline::InitPipeline() {}
	void OpenGLRenderPipeline::CleanPipeline() {}
	void OpenGLRenderPipeline::Draw() {}
	void OpenGLRenderPipeline::InitGraphics() {}
}
