#include "stpch.h"
#include "Sentinel/Graphics/Renderers/Renderer.h"

namespace Sentinel
{
	Renderer::Renderer() {
		m_RenderPipeline = RenderPipeline::Create();
	}

	Renderer::~Renderer() {}

	void Renderer::SetRenderSpecifications(const WindowProperties& windowProps,
		const FramebufferSpecification& framebufferSpecs) {
		m_RenderPipeline->CreateFrameBackings(windowProps, framebufferSpecs);
		m_RenderPipeline->CreateFrameBackingsComponents();
	}

	void Renderer::SubmitGeometryData(SharedRef<RenderResources> renderResources) {
		m_RenderPipeline->m_RenderResources = renderResources;
	}

	void Renderer::InitStartup() {
		m_RenderPipeline->InitFrameBackings();
		m_RenderPipeline->BindRenderResources();
	}

	void Renderer::PreRender() {}

	void Renderer::FramebufferBind() {
		m_RenderPipeline->BindFramebuffer();
	}

	void Renderer::DrawCommand() {
		m_RenderPipeline->Draw();
		m_RenderPipeline->SwapBuffers();
	}

	void Renderer::ClearCommand() {
		m_RenderPipeline->Clear();
	}

	void Renderer::FramebufferUnbind() {
		m_RenderPipeline->UnbindFramebuffer();
	}

	void Renderer::PostRender() {}

	void Renderer::InitShutdown() {
		m_RenderPipeline->UnbindRenderResources();
		m_RenderPipeline->CleanRenderResources();
	}

	void Renderer::Shutdown() {
		m_RenderPipeline->CleanFrameBackings();
		m_RenderPipeline->CleanNativeResources();
	}

	void Renderer::Resize(UInt32 width, UInt32 height) {
		m_RenderPipeline->Resize(width, height);
	}
}
