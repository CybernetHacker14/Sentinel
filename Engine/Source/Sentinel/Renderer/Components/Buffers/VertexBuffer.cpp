#include "stpch.h"

#include "Sentinel/Renderer/Interface/RendererAPI.h"
#include "Sentinel/Renderer/Components/Buffers/VertexBuffer.h"

#include "Platform/OpenGL/Renderer/Components/Buffers/OpenGLVertexBuffer.h"

#ifdef ST_PLATFORM_WINDOWS
#include "Platform/DirectX11/Renderer/Components/Buffers/DirectX11VertexBuffer.h"
#endif // ST_PLATFORM_WINDOWS


namespace Sentinel
{
	Ref<VertexBuffer> VertexBuffer::Create(UInt size) {

		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:
				ST_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(size);

			case RendererAPI::API::DirectX11:
			#ifdef ST_PLATFORM_WINDOWS
				return CreateRef<DirectX11VertexBuffer>(size);
			#else
				ST_ENGINE_ASSERT(false, "Can't use RendererAPI::DirectX11 on a non-Windows platform");
				return nullptr;
			#endif // ST_PLATFORM_WINDOWS
		}

		ST_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(void* vertices, UInt size) {
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:
				ST_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(vertices, size);

			case RendererAPI::API::DirectX11:
			#ifdef ST_PLATFORM_WINDOWS
				return CreateRef < DirectX11VertexBuffer>(vertices, size);
			#else
				ST_ENGINE_ASSERT(false, "Can't use RendererAPI::DirectX11 on a non-Windows platform");
				return nullptr;
			#endif // ST_PLATFORM_WINDOWS
		}

		ST_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
