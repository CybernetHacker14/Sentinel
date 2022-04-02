#pragma once

#include "Sentinel/Common/Common.h"

#include "Sentinel/Graphics/Components/Structures/RenderData.h"
#include "Sentinel/Graphics/Modules/Pipeline.h"

namespace Sentinel
{
	class RenderStageHandler {
	public:
		RenderStageHandler() = default;

		inline void ExecuteStartupStage(const WindowProperties& props) {
			if (!m_StartupStageFunction)
				return;

			m_StartupStageFunction(props);
		}

		inline void ExecuteRenderPipelinePreprocessStage() {
			if (!m_RenderPreprocessFunction)
				return;

			m_RenderPreprocessFunction();
		}

		inline void ExecuteRenderPipelineDrawStage() {
			if (!m_RenderDrawFunction)
				return;

			m_RenderDrawFunction();
		}

		inline void ExecuteRenderPipelineClearStage() {
			if (!m_RenderClearFunction)
				return;

			m_RenderClearFunction();
		}

		inline void ExecuteRenderPipelineCleanupStage() {
			if (!m_RenderCleanupFunction)
				return;

			m_RenderCleanupFunction();
		}

		inline void ExecuteShutdownStage() {
			if (!m_ShutdownStageFunction)
				return;

			m_ShutdownStageFunction();
		}

		RenderData& GetRenderData() { return *(RenderData.get()); }

	public:
		static UniqueRef<RenderStageHandler> Create();

	protected:
		STL::delegate<void(const WindowProperties&)> m_StartupStageFunction;
		STL::delegate<void()> m_RenderPreprocessFunction;
		STL::delegate<void()> m_RenderDrawFunction;
		STL::delegate<void()> m_RenderClearFunction;
		STL::delegate<void()> m_RenderCleanupFunction;
		STL::delegate<void()> m_ShutdownStageFunction;

		STL::delegate<void()> m_DestructorFunction;

	protected:
		SharedRef<RenderData> RenderData;
		SharedRef<Pipeline> RenderPipeline;

	private:
		friend class Renderer;
	};
}
