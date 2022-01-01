#pragma once

#include "Sentinel/Graphics/Modules/RenderStageHandler.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11RenderStageHandler final : public RenderStageHandlerCRTP<DX11RenderStageHandler> {
	public:
		DX11RenderStageHandler();

	private:
		void ExecuteStartupStage(const WindowProps& props);
		void ExecuteRenderPipelinePreprocessStage();
		void ExecuteRenderPipelineDrawStage();
		void ExecuteRenderPipelineCleanupStage();
		void ExecuteShutdownStage();

		void InitializeRenderData();
		void CreateWindowAndContext(const WindowProps& props);
		void InitWindowAndContext();

		void CreateAndInitRenderPipeline();
		void BindPipelineModules();
		void UnbindPipelineModules();

		void SwapBuffers();
		void Clear();
		void Draw();

		void Clean();
	private:
		friend class RenderStageHandlerCRTP<DX11RenderStageHandler>;
	};
}
