#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/Constantbuffer.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/Vertexbuffer.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/Indexbuffer.h"
#include "Sentinel/Graphics/Components/RenderResources/Buffers/VertexbufferLayout.h"

#include "Sentinel/Graphics/Components/RenderResources/Materials/Shader.h"
#include "Sentinel/Graphics/Components/RenderResources/Materials/Texture2D.h"

namespace Sentinel
{
	// TODO : Texture part needs heavy refactoring
	struct TextureTuple final : public ISharedRef {
	public:
		inline void Bind(UInt32 slot) {
			Texture->Bind(slot, ShaderType);
		}

		inline void Unbind(UInt32 slot) {
			Texture->Unbind(slot, ShaderType);
		}

	public:
		inline static SharedRef<TextureTuple> Create() {
			return CreateSharedRef<TextureTuple>();
		}

	public:
		ShaderType ShaderType;
		SharedRef<Texture2D> Texture;
	};

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
		SharedRef<TextureTuple> Textures[16];
	};
}
