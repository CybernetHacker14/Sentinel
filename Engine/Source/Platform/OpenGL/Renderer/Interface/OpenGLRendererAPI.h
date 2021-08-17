#pragma once

#include "Sentinel/Renderer/Interface/RendererAPI.h"

namespace Sentinel
{
	class OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void Init() override;
		virtual void SetViewport(UInt x, UInt y, UInt width, UInt height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
	};
}
