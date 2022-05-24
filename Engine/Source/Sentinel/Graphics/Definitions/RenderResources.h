#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/Constantbuffer.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/Vertexbuffer.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/Indexbuffer.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/VertexbufferLayout.h"

#include "Sentinel/Graphics/Components/RenderResources/Materials/Shader.h"

namespace Sentinel
{
	struct RenderResources final : public ISharedRef {
	public:
		inline static SharedRef<RenderResources> Create() {
			return CreateSharedRef<RenderResources>();
		}

	public:
		STL::vector<SharedRef<Vertexbuffer>> Vertexbuffers;
		SharedRef<Indexbuffer> Indexbuffer;
		SharedRef<VertexbufferLayout> VertexbufferLayout;
		STL::vector<SharedRef<Constantbuffer>> Constantbuffers;

		SharedRef<Shader> Shader;
	};
}
