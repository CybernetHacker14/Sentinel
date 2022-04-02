#pragma once

#include "Sentinel/Graphics/Modules/RenderStageHandler.h"
#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11RenderStageHandler final : public RenderStageHandler {
	public:
		DX11RenderStageHandler();

	private:
		void ExecuteStartupStage(const WindowProperties& props);
		void ExecuteRenderPipelinePreprocessStage();
		void ExecuteRenderPipelineDrawStage();
		void ExecuteRenderPipelineClearStage();
		void ExecuteRenderPipelineCleanupStage();
		void ExecuteShutdownStage();

		void Destructor();

	private:
		void InitializeRenderData();
		void CreateWindowAndContext(const WindowProperties& props);
		void InitWindowAndContext();

		void CreateAndInitRenderPipeline();
		void BindPipelineModules();
		void UnbindPipelineModules();

		void SwapBuffers();
		void Clear();
		void Draw();
	};
}
