#include "stpch.h"
#include "Sentinel/Renderer/Interface/RenderCommand.h"

namespace Sentinel
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
