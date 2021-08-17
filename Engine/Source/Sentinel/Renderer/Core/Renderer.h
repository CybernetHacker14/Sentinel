#pragma once

#include "Sentinel/Window/Window.h"
#include "Sentinel/Renderer/Interface/RenderCommand.h"
#include "Sentinel/Renderer/Components/RenderPipeline.h"

namespace Sentinel
{
	struct RendererData {
	public:
		Scope<Window> Window;
		Ref<RenderPipeline> RenderPipeline;
	};

	// The central point for the rendering system with the rest of the engine
	class Renderer {
	public:
		static void CreateWindowAndContext(const WindowProps& props);
		static void CreateRenderPipeline(const Scope<Window>& window);

		static void Init();
		static void Shutdown();

		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void Clear(const glm::vec4& color);

		static void OnWindowResize(UInt width, UInt height);

		static void BeginScene();
		static void EndScene();

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		static Scope<RendererData>& GetRendererData() { return m_Data; }
	private:
		static Scope<RendererData> m_Data;
	};
}
