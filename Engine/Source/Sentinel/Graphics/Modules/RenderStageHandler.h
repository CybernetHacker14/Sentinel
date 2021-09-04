#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Graphics/Components/Structures/RenderData.h"

namespace Sentinel
{
	template<typename T>
	class RenderStageHandler;

	class RenderStageHandlerBase {
	public:
		template<typename T>
		inline RenderStageHandler<T>* BaseDowncast() {
			static_assert(STL::is_base_of<RenderStageHandler<T>, T>::value,
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
			underlying().ExecuteShutdownUsage();
		}

		RenderData& GetRenderData() const { return *RenderData; }

	protected:
		Scope<RenderData> RenderData;

	private:
		friend T;
		RenderStageHandler() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};

	class RenderStageHandlerUtils {
	public:
		static Scope<RenderStageHandlerBase> Create();
	};
}
