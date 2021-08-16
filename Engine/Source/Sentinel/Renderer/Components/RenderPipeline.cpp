#include "stpch.h"
#include "Sentinel/Renderer/Components/RenderPipeline.h"
#include "Sentinel/Renderer/Interface/RendererAPI.h"
#include "Sentinel/Window/Window.h"

#include "Platform/OpenGL/Renderer/Components/OpenGLRenderPipeline.h"

#ifdef ST_PLATFORM_WINDOWS
#include "Platform/DirectX11/Renderer/Components/DirectX11RenderPipeline.h"
#endif // ST_PLATFORM_WINDOWS

namespace Sentinel
{
	Ref<RenderPipeline> RenderPipeline::Create(const Scope<Window>& window) {
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:
				ST_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<OpenGLRenderPipeline>(window);

			case RendererAPI::API::DirectX11:
			#ifdef ST_PLATFORM_WINDOWS
				return CreateRef<DirectX11RenderPipeline>(window);
			#else
				ST_ENGINE_ASSERT(false, "Can't use RendererAPI::DirectX11 on a non-Windows platform");
				return nullptr;
			#endif // ST_PLATFORM_WINDOWS
		}

		ST_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
