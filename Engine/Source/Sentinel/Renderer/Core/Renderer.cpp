#include "stpch.h"

#include "Sentinel/Renderer/Core/Renderer.h"

namespace Sentinel
{
	Scope<RendererData> Renderer::m_Data = CreateScope<RendererData>();

	void Renderer::CreateWindowAndContext(const WindowProps& props) {
		Scope<Window> window = Window::Create(props);
		RenderCommand::SetViewport(0, 0, props.Width, props.Height);
		m_Data->Window = STL::move(window);
	}

	void Renderer::CreateRenderPipeline(const Scope<Window>& window) {
		m_Data->RenderPipeline = RenderPipeline::Create(window);
	}

	void Renderer::Init() {
		RenderCommand::Init();
	}

	void Renderer::Shutdown() {}

	void Renderer::SetClearColor(const glm::vec4& color) {
		RenderCommand::SetClearColor(color);
	}

	void Renderer::Clear() {
		RenderCommand::Clear();
	}

	void Renderer::Clear(const glm::vec4& color) {
		RenderCommand::SetClearColor(color);
		RenderCommand::Clear();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene() {}
	void Renderer::EndScene() {}
}
