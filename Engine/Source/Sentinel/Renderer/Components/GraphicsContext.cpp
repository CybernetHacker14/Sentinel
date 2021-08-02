#include "stpch.h"
#include "Sentinel/Renderer/Components/GraphicsContext.h"
#include "Sentinel/Renderer/Interface/RendererAPI.h"

#include "Platform/OpenGL/Renderer/Components/OpenGLContext.h"

#ifdef ST_PLATFORM_WINDOWS
#include "Platform/DirectX11/Renderer/Components/DirectX11Context.h"
#endif // ST_PLATFORM_WINDOWS

namespace Sentinel
{
	Scope<GraphicsContext> GraphicsContext::Create(void* window) {
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:
				ST_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));

			case RendererAPI::API::DirectX11:
			#ifdef ST_PLATFORM_WINDOWS
				return CreateScope<DirectX11Context>(static_cast<GLFWwindow*>(window));
			#else
				ST_ENGINE_ASSERT(false, "Can't use RendererAPI::DirectX11 on a non-Windows platform");
				return nullptr;
			#endif // ST_PLATFORM_WINDOWS
		}

		ST_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
