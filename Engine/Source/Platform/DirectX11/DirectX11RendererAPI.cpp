#include "stpch.h"
#include "Platform/DirectX11/DirectX11RendererAPI.h"
#include "Platform/DirectX11/DirectX11Internal.h"

#include <glm/gtc/type_ptr.hpp>

namespace Sentinel
{
	void DirectX11RendererAPI::Init() {}

	void DirectX11RendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		D3D11_VIEWPORT viewport;

		viewport.TopLeftX = x;
		viewport.TopLeftY = y;
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;

		DirectX11Internal::GetInternalHandle()->GetDirectX11DeviceContext()->RSSetViewports(1, &viewport);
	}

	void DirectX11RendererAPI::SetClearColor(const glm::vec4& color) {
		m_ClearColor = color;
	}

	void DirectX11RendererAPI::Clear() {
		DirectX11Internal::GetInternalHandle()->GetDirectX11DeviceContext()->ClearRenderTargetView(
			DirectX11Internal::GetInternalHandle()->GetDirectX11RenderTargetView().Get(), glm::value_ptr(m_ClearColor));
	}
}
