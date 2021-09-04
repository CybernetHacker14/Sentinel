#include "stpch.h"

#include "Sentinel/Graphics/Renderers/Renderer.h"

namespace Sentinel
{
	Renderer::Renderer(Ref<DeviceModules> deviceModules, Ref<PipelineModules> pipelineModules) {
		m_RenderStageHandler = RenderStageHandlerUtils::Create();

		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
			case Backend::API::DirectX11:
				Init<DX11RenderStageHandler>(deviceModules, pipelineModules);
		}
	}

	Renderer::~Renderer() {

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

	template<typename T>
	void Renderer::Init(Ref<DeviceModules> deviceModules, Ref<PipelineModules> pipelineModules) {
		m_RenderStageHandler->BaseDowncast<T>()->GetRenderData().DeviceModules = deviceModules;
		m_RenderStageHandler->BaseDowncast<T>()->GetRenderData().PipelineModules = pipelineModules;
		m_RenderStageHandler->BaseDowncast<T>()->ExecuteStartupStage(
			*(m_RenderStageHandler->BaseDowncast<T>()->GetRenderData().DeviceModules->WindowProps)
		);
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
