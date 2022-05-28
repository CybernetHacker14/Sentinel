#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/Constantbuffer.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/Vertexbuffer.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/Indexbuffer.h"

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
		STL::vector<SharedRef<Constantbuffer>> Constantbuffers;
	};
}
