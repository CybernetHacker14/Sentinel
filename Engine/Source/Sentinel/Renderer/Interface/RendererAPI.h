#pragma once

#include "Sentinel/Base/Define.h"
#include <glm/glm.hpp>

namespace Sentinel
{
	class RendererAPI {
	public:
		enum class API {
			None = 0, OpenGL = 1, DirectX11 = 2, Vulkan = 3
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(UInt x, UInt y, UInt width, UInt height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();
	private:
		static API s_API;
	};
}