#include "stpch.h"
#include "Sentinel/Renderer/Components/RenderPipeline.h"
#include "Sentinel/Renderer/Interface/RendererAPI.h"
#include "Sentinel/Window/Window.h"

namespace Sentinel
{
	Scope<RenderPipeline> RenderPipeline::Create(void* window) {
		return Scope<RenderPipeline>();
	}
}
