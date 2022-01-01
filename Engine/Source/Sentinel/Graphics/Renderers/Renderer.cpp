#include "stpch.h"
#include "Sentinel/Graphics/Renderers/Renderer.h"

namespace Sentinel
{
	Renderer::Renderer(SharedRef<DeviceModules> deviceModules) {
		m_RenderStageHandler = STL::move(RenderStageHandler::Create());
		m_RenderStageHandler->RenderData->DeviceModules = deviceModules;
		m_RenderStageHandler->ExecuteStartupStage(*(m_RenderStageHandler->RenderData->DeviceModules->WindowProps));
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

	void Renderer::Shutdown() {
		m_RenderStageHandler->ExecuteRenderPipelineCleanupStage();
	}

	SharedRef<DeviceModules> Renderer::GetDeviceModulesFromRenderData() {
		return m_RenderStageHandler->GetRenderData().DeviceModules;
	}

	SharedRef<PipelineModules> Renderer::GetPipelineModulesFromRenderData() {
		return m_RenderStageHandler->GetRenderData().PipelineModules;
	}
}
