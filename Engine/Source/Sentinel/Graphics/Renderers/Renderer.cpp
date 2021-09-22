#include "stpch.h"

#include "Sentinel/Graphics/Renderers/Renderer.h"

namespace Sentinel
{
	Renderer::Renderer(Ref<DeviceModules> deviceModules) {
		m_RenderStageHandler = RenderStageHandler::Create();
		m_RenderStageHandler->RenderData->DeviceModules = deviceModules;
		m_RenderStageHandler->ExecuteStartupStage(*(m_RenderStageHandler->RenderData->DeviceModules->WindowProps));
	}

	Renderer::~Renderer() {}

	void Renderer::SetPipelineData(Ref<PipelineModules> pipelineModules) {
		m_RenderStageHandler->RenderData->PipelineModules = pipelineModules;
		m_RenderStageHandler->ExecuteRenderPipelinePreprocessStage();
	}

	Window& Renderer::GetWindow() {
		return *(m_RenderStageHandler->GetRenderData().DeviceModules->Window);
	}

	void Renderer::Draw() {
		m_RenderStageHandler->ExecuteRenderPipelineDrawStage();
	}

	Ref<DeviceModules> Renderer::GetDeviceModulesFromRenderData() const {
		return m_RenderStageHandler->GetRenderData().DeviceModules;
	}

	Ref<PipelineModules> Renderer::GetPipelineModulesFromRenderData() const {
		return m_RenderStageHandler->GetRenderData().PipelineModules;
	}
}
