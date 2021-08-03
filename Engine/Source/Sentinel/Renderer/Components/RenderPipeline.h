#pragma once

namespace Sentinel
{
	class RenderPipeline {
	public:
		virtual void InitPipeline() = 0;
		virtual void CleanPipeline() = 0;

		static Scope<RenderPipeline> Create(void* window);
	};
}
