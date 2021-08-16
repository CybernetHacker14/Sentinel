#include "stpch.h"
#include "Platform/DirectX11/Renderer/Components/DirectX11RenderPipeline.h"
#include "Platform/DirectX11/Renderer/Core/DirectX11Internal.h"

namespace Sentinel
{
	DirectX11RenderPipeline::DirectX11RenderPipeline(const Scope<Window>& window) {
		m_NativeWindow = (GLFWwindow*)window->GetNativeWindow();
	}

	DirectX11RenderPipeline::~DirectX11RenderPipeline() {
		DirectX11Internal::GetInternalHandle()->ReleaseAndCleanObjects();
	}

	void DirectX11RenderPipeline::InitPipeline() {
		// Shaders, Vertexbuffers, Indexbuffers, VertexArrays, everything needs to be setup
		DirectX11Internal::GetInternalHandle()->CreateDeviceAndSwapchain(m_NativeWindow);
	}

	void DirectX11RenderPipeline::CleanPipeline() {
		DirectX11Internal::GetInternalHandle()->ReleaseAndCleanObjects();
	}

	void DirectX11RenderPipeline::Draw() {}

	void DirectX11RenderPipeline::InitGraphics() {}

	void DirectX11RenderPipeline::InitSwapChainTargetView() {

	}
}
