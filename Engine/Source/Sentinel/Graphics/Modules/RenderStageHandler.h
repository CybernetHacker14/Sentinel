#pragma once

#include "Sentinel/Base/Define.h"

#include "Sentinel/Window/Window.h"
#include "Sentinel/Graphics/Modules/GraphicsContext.h"

namespace Sentinel
{
	template<typename T>
	class RenderStageHandler;

	class RenderStageHandlerBase {
	public:
		template<typename T>
		inline RenderStageHandler<T>* BaseDowncast() {
			static_assert(STL::is_base_of<RenderStageHandler<T>, T>::value
				"Operation not allowed. 'T' should be a derived from RenderStageHandler<T>.");
			return static_cast<RenderStageHandler<T>*>(this);
		}

		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<RenderStageHandler<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from RenderStageHandler<T>.");
			return static_cast<T*>(this);
		}
	};

	template<typename T>
	class RenderStageHandler : public RenderStageHandlerBase {
	public:
		void ExecuteStartupStage();
		void ExecuteRenderPipelinePreprocessStage();
		void ExecuteRenderPipelineDrawStage();
		void ExecuteRenderPipelineCleanupStage();
		void ExecuteShutdownStage();

	protected:
		void CreateWindowAndContext();
		void InitializeDevices();
		void SetViewport(UInt x, UInt y, UInt width, UInt height);
		void InitializeSwapchain();
		void InitializeRenderData();
		void SetRenderTargets();

		void SwapBuffers();
		void Draw();
	};
}
