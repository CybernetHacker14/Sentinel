#include "stpch.h"
#include "Sentinel/Renderer/Core/RenderCommand.h"

namespace Sentinel
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
