#include "stpch.h"
#include "Sentinel/Graphics/Renderers/Renderer.h"

namespace Sentinel
{
	Renderer::Renderer(SharedRef<DeviceModules> deviceModules) {
		m_RenderStageHandler = STL::move(RenderStageHandler::Create());
		m_RenderStageHandler->RenderData->DeviceModules = deviceModules;
		m_RenderStageHandler->ExecuteStartupStage(*(m_RenderStageHandler->RenderData->DeviceModules->WindowProperties));
	}

	Renderer::~Renderer() {
		m_RenderStageHandler->ExecuteShutdownStage();
	}

	void Renderer::SetPipelineData(SharedRef<PipelineModules> pipelineModules) {
		m_RenderStageHandler->RenderData->PipelineModules = pipelineModules;
		m_RenderStageHandler->ExecuteRenderPipelinePreprocessStage();
	}

	Window& Renderer::GetWindow() {
		return *(m_RenderStageHandler->GetRenderData().DeviceModules->Window);
	}

	void Renderer::Draw() {
		m_RenderStageHandler->ExecuteRenderPipelineDrawStage();
	}

	void Renderer::Clear() {
		m_RenderStageHandler->ExecuteRenderPipelineClearStage();
	}

	void Renderer::Shutdown() {
		m_RenderStageHandler->ExecuteRenderPipelineCleanupStage();
	}

	void Renderer::Resize(UInt32 width, UInt32 height) {
		m_RenderStageHandler->Resize(width, height);
	}

	void Renderer::FramebufferBind() {
		m_RenderStageHandler->FramebufferBindFunction();
	}

	void Renderer::FramebufferUnbind() {
		m_RenderStageHandler->FramebufferUnbindFunction();
	}

	SharedRef<DeviceModules> Renderer::GetDeviceModulesFromRenderData() {
		return m_RenderStageHandler->GetRenderData().DeviceModules;
	}

	SharedRef<PipelineModules> Renderer::GetPipelineModulesFromRenderData() {
		return m_RenderStageHandler->GetRenderData().PipelineModules;
	}
}
