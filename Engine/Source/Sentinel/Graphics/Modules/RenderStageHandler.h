#pragma once

#include "Sentinel/Base/Define.h"

#include "Sentinel/Graphics/Components/Structures/RenderData.h"
#include "Sentinel/Graphics/Modules/Pipeline.h"

namespace Sentinel
{
	template<typename T>
	class RenderStageHandlerCRTP;

	class RenderStageHandler {
	public:
		void ExecuteStartupStage(const WindowProps& props);
		void ExecuteRenderPipelinePreprocessStage();
		void ExecuteRenderPipelineDrawStage();
		void ExecuteRenderPipelineCleanupStage();
		void ExecuteShutdownStage();

		RenderData& GetRenderData() { return *(RenderData.get()); }

		static Scope<RenderStageHandler> Create();

	protected:
		RenderStageHandler() = default;

	protected:
		Ref<RenderData> RenderData;
		Ref<Pipeline> RenderPipeline;

	private:
		friend class Renderer;

	private:
		template<typename T>
		inline RenderStageHandlerCRTP<T>* BaseDowncast() {
			static_assert(STL::is_base_of<RenderStageHandlerCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from RenderStageHandlerCRTP<T>.");
			return static_cast<RenderStageHandlerCRTP<T>*>(this);
		}

		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<RenderStageHandlerCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from RenderStageHandlerCRTP<T>.");
			return static_cast<T*>(this);
		}
	};

	template<typename T>
	class RenderStageHandlerCRTP : public RenderStageHandler {
	private:
		inline void ExecuteStartupStage(const WindowProps& props) {
			underlying().ExecuteStartupStage(props);
		}

		inline void ExecuteRenderPipelinePreprocessStage() {
			underlying().ExecuteRenderPipelinePreprocessStage();
		}

		inline void ExecuteRenderPipelineDrawStage() {
			underlying().ExecuteRenderPipelineDrawStage();
		}

		inline void ExecuteRenderPipelineCleanupStage() {
			underlying().ExecuteRenderPipelineCleanupStage();
		}

		inline void ExecuteShutdownStage() {
			underlying().ExecuteShutdownStage();
		}

	private:
		friend T;
		friend RenderStageHandler;
		RenderStageHandlerCRTP() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};
}
