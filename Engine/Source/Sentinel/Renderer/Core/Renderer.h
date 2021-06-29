#pragma once

#include "Sentinel/Renderer/Core/RenderCommand.h"

namespace Sentinel
{
	// The central point for the rendering system with the rest of the engine
	class Renderer {
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene();
		static void EndScene();

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}
