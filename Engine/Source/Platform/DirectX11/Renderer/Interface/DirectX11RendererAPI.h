#pragma once

#include "Sentinel/Renderer/Interface/RendererAPI.h"

namespace Sentinel
{
	class DirectX11RendererAPI : public RendererAPI {
	public:
		virtual void Init() override;
		virtual void SetViewport(UInt x, UInt y, UInt width, UInt height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

	private:
		glm::vec4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	};
}
