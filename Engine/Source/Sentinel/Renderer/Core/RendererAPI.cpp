#include "stpch.h"
#include "Sentinel/Renderer/Core/RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/DirectX11/DirectX11RendererAPI.h"

namespace Sentinel
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::DirectX11;

	Scope<RendererAPI> RendererAPI::Create() {
		switch (s_API)
		{
			case RendererAPI::API::None: ST_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateScope<OpenGLRendererAPI>();
			case RendererAPI::API::DirectX11: return CreateScope<DirectX11RendererAPI>();
		}

		ST_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
