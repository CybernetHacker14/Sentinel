#include "stpch.h"
#include "Platform/DirectX11/Renderer/Interface/DirectX11RendererAPI.h"
#include "Platform/DirectX11/Renderer/Core/DirectX11Internal.h"

#include <glm/gtc/type_ptr.hpp>

namespace Sentinel
{
	void DirectX11RendererAPI::Init() {}

	void DirectX11RendererAPI::SetViewport(UInt x, UInt y, UInt width, UInt height) {
		DirectX11Internal::GetInternalHandle()->SetViewport(x, y, width, height);
	}

	void DirectX11RendererAPI::SetClearColor(const glm::vec4& color) {
		m_ClearColor = color;
	}

	void DirectX11RendererAPI::Clear() {
		DirectX11Internal::GetInternalHandle()->ClearColor((float*)&m_ClearColor);
	}
}
