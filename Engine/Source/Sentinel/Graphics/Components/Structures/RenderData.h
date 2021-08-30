#pragma once

#include "Sentinel/Base/Define.h"

#include "Sentinel/Window/Window.h"
#include "Sentinel/Graphics/Modules/GraphicsContext.h"

#include "Sentinel/Graphics/Components/Buffers/Vertexbuffer.h"
#include "Sentinel/Graphics/Components/Buffers/Indexbuffer.h"

namespace Sentinel
{
	struct RenderPipelineModules {
	public:
		STL::vector<Ref<VertexbufferBase>> Vertexbuffers;
		Ref<IndexbufferBase> Indexbuffer;
	};

	struct RendererModules {
	public:
		Scope<Window> Window;
		Scope<GraphicsContextBase> GraphicsContext;
	};

	struct RenderData {

	};
}
