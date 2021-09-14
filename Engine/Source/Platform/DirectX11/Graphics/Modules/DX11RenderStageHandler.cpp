#include "stpch.h"

#include "Platform/DirectX11/Graphics/Modules/DX11GraphicsContext.h"
#include "Platform/DirectX11/Graphics/Modules/DX11Pipeline.h"
#include "Platform/DirectX11/Graphics/Modules/DX11RenderStageHandler.h"

#include "Platform/DirectX11/Graphics/Components/Materials/DX11Shader.h"
#include "Platform/DirectX11/Graphics/Components/Buffers/DX11Vertexbuffer.h"
#include "Platform/DirectX11/Graphics/Components/Buffers/DX11Indexbuffer.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Sentinel
{
	DX11RenderStageHandler::DX11RenderStageHandler() {
		InitializeRenderData();
	}

	DX11RenderStageHandler::~DX11RenderStageHandler() {
		if (m_RenderTargetView)
		{
			m_RenderTargetView->Release();
		}
	}

	void DX11RenderStageHandler::ExecuteStartupStage(const WindowProps& props) {
		CreateWindowAndContext(props);
		InitializeSwapchain();
		InitializeDevices();
		SetViewport(0, 0, props.Width, props.Height);
	}

	void DX11RenderStageHandler::ExecuteRenderPipelinePreprocessStage() {
		RenderPipeline = PipelineUtils::Create();
		RenderPipeline->BaseDowncast<DX11Pipeline>()->CreateInputLayout(
			RenderData->PipelineModules->Shader
		);

		for (auto& vBuffer : RenderData->PipelineModules->Vertexbuffers)
			vBuffer->DerivedDowncast<DX11Vertexbuffer>()->Bind(
				RenderPipeline->BaseDowncast<DX11Pipeline>()->GetStride()
			);

		RenderData->PipelineModules->Indexbuffer->BaseDowncast<DX11Indexbuffer>()->Bind();
		RenderData->PipelineModules->Shader->BaseDowncast<DX11Shader>()->Bind();

		SetRenderTargets();

		DX11Common::GetContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		RenderPipeline->BaseDowncast<DX11Pipeline>()->Bind();
	}

	void DX11RenderStageHandler::ExecuteRenderPipelineDrawStage() {
		Clear();
		Draw();
		SwapBuffers();
	}

	void DX11RenderStageHandler::ExecuteRenderPipelineCleanupStage() {}

	void DX11RenderStageHandler::ExecuteShutdownStage() {}

	/////////////////////////////////////////////////////////////////////////////

	void DX11RenderStageHandler::InitializeRenderData() {
		RenderData = Sentinel::CreateRef<Sentinel::RenderData>();
	}

	void DX11RenderStageHandler::CreateWindowAndContext(const WindowProps& props) {
		RenderData->DeviceModules->Window = Window::Create(props);
		RenderData->DeviceModules->GraphicsContext =
			GraphicsContextUtils::Create(
				static_cast<GLFWwindow*>(RenderData->DeviceModules->Window->GetNativeWindow()));
	}

	void DX11RenderStageHandler::InitializeSwapchain() {
		DXGI_SWAP_CHAIN_DESC swapChainDescription;
		ZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDescription.BufferDesc.Width = 0;
		swapChainDescription.BufferDesc.Height = 0;
		swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDescription.BufferDesc.RefreshRate.Denominator = 0;
		swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		swapChainDescription.SampleDesc.Count = 4;
		swapChainDescription.SampleDesc.Quality = 0;

		swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		swapChainDescription.BufferCount = 1;

		swapChainDescription.OutputWindow =
			glfwGetWin32Window(static_cast<GLFWwindow*>(RenderData->DeviceModules->Window->GetNativeWindow()));

		swapChainDescription.Windowed = true;

		swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDescription.Flags = 0;

		auto result = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&swapChainDescription,
			&(DX11Common::m_Swapchain),
			&(DX11Common::m_Device),
			nullptr,
			&(DX11Common::m_Context)
		);
	}

	void DX11RenderStageHandler::InitializeDevices() {
		DX11Common::GetDevice()->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&(DX11Common::m_DXGIDevice));
		DX11Common::GetDXGIDevice()->GetParent(__uuidof(IDXGIAdapter), (LPVOID*)&(DX11Common::m_Adapter));

		RenderData->DeviceModules->GraphicsContext->BaseDowncast<DX11GraphicsContext>()->Init();
		RenderData->DeviceModules->Window->SetVSync(true);
		ContextInfo info = RenderData->DeviceModules->GraphicsContext->BaseDowncast<DX11GraphicsContext>()->GetContextInfo();

		ST_ENGINE_INFO("VENDOR   : {0}", info.Vendor.c_str());
		ST_ENGINE_INFO("RENDERER : {0}", info.Renderer.c_str());
		ST_ENGINE_INFO("API      : {0}", info.API.c_str());
		ST_ENGINE_INFO("VERSION  : {0}", info.Version.c_str());
	}

	void DX11RenderStageHandler::SetViewport(UInt x, UInt y, UInt width, UInt height) {
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = x;
		viewport.TopLeftY = y;
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;

		DX11Common::GetContext()->RSSetViewports(1, &viewport);
	}

	void DX11RenderStageHandler::SetRenderTargets() {
		ID3D11Texture2D* backBuffer = nullptr;

		// Get the address of the back buffer
		DX11Common::GetSwapchain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

		// use the back buffer address to create the render target
		DX11Common::GetDevice()->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView);
		backBuffer->Release();

		DX11Common::GetContext()->OMSetRenderTargets(1, &m_RenderTargetView, nullptr);
	}

	void DX11RenderStageHandler::SwapBuffers() {
		DX11Common::GetSwapchain()->Present(0, 0);
	}

	void DX11RenderStageHandler::Clear() {
		DX11Common::GetContext()->ClearRenderTargetView(m_RenderTargetView, (float*)&(RenderData->PipelineModules->ClearColor));
	}

	void DX11RenderStageHandler::Draw() {
		DX11Common::GetContext()->DrawIndexed(
			RenderData->PipelineModules->Indexbuffer->BaseDowncast<DX11Indexbuffer>()->GetCount(), 0, 0);
	}
}
