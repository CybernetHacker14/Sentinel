#pragma once

#include "Sentinel/Renderer/Core/RendererAPI.h"

namespace Sentinel
{
	class DirectX11RendererAPI : public RendererAPI {
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

	private:
		glm::vec4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	};
}
