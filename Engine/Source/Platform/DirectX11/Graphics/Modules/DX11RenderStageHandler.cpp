#include "stpch.h"
#include "Platform/DirectX11/Graphics/Modules/DX11RenderStageHandler.h"

namespace Sentinel
{
	DX11RenderStageHandler::DX11RenderStageHandler() {
		InitializeRenderData();

		m_StartupStageFunction = ST_BIND_EVENT_FN(ExecuteStartupStage);
		m_RenderPreprocessFunction = ST_BIND_EVENT_FN(ExecuteRenderPipelinePreprocessStage);
		m_RenderDrawFunction = ST_BIND_EVENT_FN(ExecuteRenderPipelineDrawStage);
		m_RenderClearFunction = ST_BIND_EVENT_FN(ExecuteRenderPipelineClearStage);
		m_RenderCleanupFunction = ST_BIND_EVENT_FN(ExecuteRenderPipelineCleanupStage);
		m_ShutdownStageFunction = ST_BIND_EVENT_FN(ExecuteShutdownStage);

		m_DestructorFunction = ST_BIND_EVENT_FN(Destructor);
	}

	void DX11RenderStageHandler::ExecuteStartupStage(const WindowProperties& props) {
		CreateWindowAndContext(props);
		InitWindowAndContext();
	}

	void DX11RenderStageHandler::ExecuteRenderPipelinePreprocessStage() {
		CreateAndInitRenderPipeline();
		BindPipelineModules();
	}

	void DX11RenderStageHandler::ExecuteRenderPipelineDrawStage() {
		Draw();
		SwapBuffers();
	}

	void DX11RenderStageHandler::ExecuteRenderPipelineClearStage() {
		Clear();
	}

	void DX11RenderStageHandler::ExecuteRenderPipelineCleanupStage() {
		UnbindPipelineModules();
		RenderPipeline->Unbind();
	}

	void DX11RenderStageHandler::ExecuteShutdownStage() {
		RenderPipeline->Clean();
		for (auto& vBuffer : RenderData->PipelineModules->Vertexbuffers)
		{
			if (vBuffer)
				vBuffer->Clean();
		}
		RenderData->PipelineModules->Indexbuffer->Clean();
		RenderData->PipelineModules->Shader->Clean();
		DX11Common::m_Adapter->Release();
		DX11Common::m_DXGIDevice->Release();
		DX11Common::m_Swapchain->Release();
		DX11Common::m_Device->Release();
		DX11Common::m_Context->Release();
	}

	void DX11RenderStageHandler::Destructor() {

	}

	/////////////////////////////////////////////////////////////////////////////

	void DX11RenderStageHandler::InitializeRenderData() {
		RenderData = Sentinel::CreateSharedRef<Sentinel::RenderData>();
	}

	void DX11RenderStageHandler::CreateWindowAndContext(const WindowProperties& props) {
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

	void DX11RenderStageHandler::CreateAndInitRenderPipeline() {
		RenderPipeline = Pipeline::Create();
		RenderPipeline->CreateInputLayout(RenderData->PipelineModules->Shader);
		RenderPipeline->Bind();
	}

	void DX11RenderStageHandler::BindPipelineModules() {
		for (auto& vBuffer : RenderData->PipelineModules->Vertexbuffers)
		{
			if (vBuffer)
				vBuffer->Bind(RenderPipeline->GetStride());
		}

		if (RenderData->PipelineModules->Indexbuffer)
			RenderData->PipelineModules->Indexbuffer->Bind();

		RenderData->PipelineModules->Shader->Bind();
		RenderData->PipelineModules->Framebuffer->Bind();
	}

	void DX11RenderStageHandler::UnbindPipelineModules() {
		for (auto& vBuffer : RenderData->PipelineModules->Vertexbuffers)
		{
			if (vBuffer)
				vBuffer->Unbind();
		}
		RenderData->PipelineModules->Shader->Reset();
		RenderData->PipelineModules->Indexbuffer->Unbind();
		RenderData->PipelineModules->Framebuffer->Unbind();
	}

	void DX11RenderStageHandler::SwapBuffers() {
		DX11Common::GetSwapchain()->Present(1, 0);
	}

	void DX11RenderStageHandler::Clear() {
		RenderData->PipelineModules->Framebuffer->Clear();
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
