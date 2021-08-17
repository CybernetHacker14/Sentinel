#include "stpch.h"

#include "Sentinel/Renderer/Interface/RendererAPI.h"
#include "Sentinel/Renderer/Components/Buffers/IndexBuffer.h"

#include "Platform/OpenGL/Renderer/Components/Buffers/OpenGLIndexBuffer.h"

#ifdef ST_PLATFORM_WINDOWS
#include "Platform/DirectX11/Renderer/Components/Buffers/DirectX11IndexBuffer.h"
#endif // ST_PLATFORM_WINDOWS


namespace Sentinel
{
	Ref<IndexBuffer> IndexBuffer::Create(void* indices, UInt count) {

		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:
				ST_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLIndexBuffer>(indices, count);

			case RendererAPI::API::DirectX11:
			#ifdef ST_PLATFORM_WINDOWS
				return CreateRef<DirectX11IndexBuffer>(indices, count);
			#else
				ST_ENGINE_ASSERT(false, "Can't use RendererAPI::DirectX11 on a non-Windows platform");
				return nullptr;
			#endif // ST_PLATFORM_WINDOWS
		}

		ST_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
