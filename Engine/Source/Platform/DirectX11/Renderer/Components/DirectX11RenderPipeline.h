#pragma once

#include "Sentinel/Renderer/Components/RenderPipeline.h"

struct GLFWwindow;

namespace Sentinel
{
	class DirectX11RenderPipeline : public RenderPipeline {
	public:
		DirectX11RenderPipeline(const Scope<Window>& window);
		virtual ~DirectX11RenderPipeline();

		virtual void InitPipeline() override;
		virtual void CleanPipeline() override;

		virtual void Draw() override;
	protected:
		virtual void InitGraphics() override;
		virtual void InitSwapChainTargetView() override;
	private:
		GLFWwindow* m_NativeWindow;
	};
}