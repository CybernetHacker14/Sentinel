#pragma once

#include "Sentinel/Renderer/Interface/RendererAPI.h"

namespace Sentinel
{
	class RenderCommand {
	public:
		static void Init() {
			s_RendererAPI->Init();
		}

		static void SetViewport(UInt x, UInt y, UInt width, UInt height) {
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}

		static void Clear() {
			s_RendererAPI->Clear();
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}
