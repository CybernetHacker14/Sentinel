#pragma once

#include "Sentinel/Renderer/Components/RenderPipeline.h"

namespace Sentinel
{
	class DirectX11RenderPipeline : public RenderPipeline {
		virtual void InitPipeline() override;
		virtual void CleanPipeline() override;
	};
}
