#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Window/Window.h"

namespace Sentinel
{
	class RenderPipeline : public IntrusiveRefObject {
	public:
		virtual void InitPipeline() = 0;
		virtual void CleanPipeline() = 0;

		virtual void Draw() = 0;

		static Ref<RenderPipeline> Create(const Scope<Window>& window);
	protected:
		virtual void InitGraphics() = 0;
		virtual void InitSwapChainTargetView() = 0;
	};
}
