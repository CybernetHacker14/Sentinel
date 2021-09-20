#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Graphics/Modules/RenderStageHandler.h"

#include "Platform/DirectX11/Graphics/Core/DX11Common.h"

namespace Sentinel
{
	class DX11RenderStageHandler : public RenderStageHandlerCRTP<DX11RenderStageHandler> {
	public:
		DX11RenderStageHandler();
		~DX11RenderStageHandler();

		void ExecuteStartupStage(const WindowProps& props);
		void ExecuteRenderPipelinePreprocessStage();
		void ExecuteRenderPipelineDrawStage();
		void ExecuteRenderPipelineCleanupStage();
		void ExecuteShutdownStage();

	private:
		void InitializeRenderData();

		void CreateWindowAndContext(const WindowProps& props);
		void InitWindowAndContext();
		void SetViewport(UInt x, UInt y, UInt width, UInt height);

		void CreateAndInitRenderPipeline();
		void BindPipelineModules();
		void SetRenderTargets();

		void SwapBuffers();
		void Clear();
		void Draw();

	private:
		// TODO: Move this to a separate Framebuffer class
		// And make this data a part of RenderData::PipelineModules

		ID3D11RenderTargetView* m_RenderTargetView;
	};
}
