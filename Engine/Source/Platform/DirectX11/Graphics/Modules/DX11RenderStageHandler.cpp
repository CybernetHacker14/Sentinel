#include "stpch.h"

#include "Platform/DirectX11/Graphics/Modules/DX11RenderStageHandler.h"

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
		InitWindowAndContext();
		SetViewport(0, 0, props.Width, props.Height);
	}

	void DX11RenderStageHandler::ExecuteRenderPipelinePreprocessStage() {
		CreateAndInitRenderPipeline();
		BindPipelineModules();
		SetRenderTargets();
	}

	void DX11RenderStageHandler::ExecuteRenderPipelineDrawStage() {
		Clear();
		Draw();
		SwapBuffers();
	}

	void DX11RenderStageHandler::ExecuteRenderPipelineCleanupStage() {
		m_RenderTargetView->Release();
		DX11Common::m_Adapter->Release();
		DX11Common::m_DXGIDevice->Release();
		DX11Common::m_Swapchain->Release();
		DX11Common::m_Device->Release();
		DX11Common::m_Context->Release();
	}

	void DX11RenderStageHandler::ExecuteShutdownStage() {}

	/////////////////////////////////////////////////////////////////////////////

	void DX11RenderStageHandler::InitializeRenderData() {
		RenderData = Sentinel::CreateRef<Sentinel::RenderData>();
	}

	void DX11RenderStageHandler::CreateWindowAndContext(const WindowProps& props) {
		RenderData->DeviceModules->Window = Window::Create(props);
		RenderData->DeviceModules->GraphicsContext =
			GraphicsContext::Create(
				static_cast<GLFWwindow*>(RenderData->DeviceModules->Window->GetNativeWindow()));
	}

	void DX11RenderStageHandler::InitWindowAndContext() {
		RenderData->DeviceModules->GraphicsContext->Init();
		RenderData->DeviceModules->Window->SetVSync(true);
		ContextInfo info = RenderData->DeviceModules->GraphicsContext->GetContextInfo();

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

	void DX11RenderStageHandler::CreateAndInitRenderPipeline() {
		RenderPipeline = Pipeline::Create();
		RenderPipeline->CreateInputLayout(RenderData->PipelineModules->Shader);
		RenderPipeline->Bind();
	}

	void DX11RenderStageHandler::BindPipelineModules() {
		for (auto& vBuffer : RenderData->PipelineModules->Vertexbuffers)
			if (vBuffer)
			{
				vBuffer->Bind(
					RenderPipeline->GetStride()
				);
			}

		if (RenderData->PipelineModules->Indexbuffer)
			RenderData->PipelineModules->Indexbuffer->Bind();

		RenderData->PipelineModules->Shader->Bind();
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
		if (RenderData->PipelineModules->Indexbuffer)
		{
			DX11Common::GetContext()->DrawIndexed(
				RenderData->PipelineModules->Indexbuffer->GetCount(), 0, 0);
		}
		else
		{
			DX11Common::GetContext()->Draw(0, 0);
		}
	}
}
