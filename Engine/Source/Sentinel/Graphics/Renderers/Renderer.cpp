#include "stpch.h"

#include "Sentinel/Graphics/Renderers/Renderer.h"

namespace Sentinel
{
	Renderer::Renderer(Ref<DeviceModules> deviceModules) {
		m_RenderStageHandler = RenderStageHandlerUtils::Create();

		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
			case Backend::API::DirectX11:
				InitDevices<DX11RenderStageHandler>(deviceModules);
		}
	}

	Renderer::~Renderer() {}

	void Renderer::SetPipelineData(Ref<PipelineModules> pipelineModules) {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
			case Backend::API::DirectX11:
				InitPipeline<DX11RenderStageHandler>(pipelineModules);
		}
	}

	Window& Renderer::GetWindow() {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
			case Backend::API::DirectX11:
				return *(m_RenderStageHandler->BaseDowncast<DX11RenderStageHandler>()->GetRenderData().DeviceModules->Window);
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
	}

	void Renderer::Draw() {
		m_RenderStageHandler->BaseDowncast<DX11RenderStageHandler>()->ExecuteRenderPipelineDrawStage();
	}

	template<typename T>
	void Renderer::InitDevices(Ref<DeviceModules> deviceModules) {
		m_RenderStageHandler->BaseDowncast<T>()->RenderData->DeviceModules = deviceModules;
		m_RenderStageHandler->BaseDowncast<T>()->ExecuteStartupStage(
			*(m_RenderStageHandler->BaseDowncast<T>()->GetRenderData().DeviceModules->WindowProps)
		);
	}

	template<typename T>
	void Renderer::InitPipeline(Ref<PipelineModules> pipelineModules) {
		m_RenderStageHandler->BaseDowncast<T>()->RenderData->PipelineModules = pipelineModules;
		m_RenderStageHandler->BaseDowncast<T>()->ExecuteRenderPipelinePreprocessStage();
	}

	Ref<DeviceModules> Renderer::GetDeviceModulesFromRenderData() const {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None is currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				m_RenderStageHandler->BaseDowncast<DX11RenderStageHandler>()->GetRenderData().DeviceModules;
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}

	Ref<PipelineModules> Renderer::GetPipelineModulesFromRenderData() const {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None is currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				m_RenderStageHandler->BaseDowncast<DX11RenderStageHandler>()->GetRenderData().PipelineModules;
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
