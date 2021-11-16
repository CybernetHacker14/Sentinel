#include "stpch.h"
#include "Sentinel/Graphics/Core/Backend.h"
#include "Sentinel/Graphics/Modules/RenderStageHandler.h"

#include "Platform/DirectX11/Graphics/Modules/DX11RenderStageHandler.h"

namespace Sentinel
{
	void RenderStageHandler::ExecuteStartupStage(const WindowProps& props) {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11RenderStageHandler>()->ExecuteStartupStage(props);
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
		}
	}

	void RenderStageHandler::ExecuteRenderPipelinePreprocessStage() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11RenderStageHandler>()->ExecuteRenderPipelinePreprocessStage();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
		}
	}

	void RenderStageHandler::ExecuteRenderPipelineDrawStage() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11RenderStageHandler>()->ExecuteRenderPipelineDrawStage();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
		}
	}

	void RenderStageHandler::ExecuteRenderPipelineCleanupStage() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11RenderStageHandler>()->ExecuteRenderPipelineCleanupStage();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
		}
	}

	void RenderStageHandler::ExecuteShutdownStage() {
		if (Backend::GetAPI() == Backend::API::DirectX11)
		{
			BaseDowncast<DX11RenderStageHandler>()->ExecuteShutdownStage();
		}
		else if (Backend::GetAPI() == Backend::API::None)
		{
			ST_ENGINE_ASSERT(false, "API::None currently not supported");
		}
		else
		{
			ST_ENGINE_ASSERT(false, "Unknown Backend API");
		}
	}

	UniqueRef<RenderStageHandler> RenderStageHandler::Create() {
		switch (Backend::GetAPI())
		{
			case Backend::API::None:
				ST_ENGINE_ASSERT(false, "API::None currently not supported");
				return nullptr;
			case Backend::API::DirectX11:
				return CreateUniqueRef<DX11RenderStageHandler>();
		}

		ST_ENGINE_ASSERT(false, "Unknown Backend API");
		return nullptr;
	}
}
