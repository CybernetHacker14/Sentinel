#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Graphics/Core/Backend.h"
#include "Sentinel/Graphics/Modules/RenderStageHandler.h"

#include "Platform/DirectX11/Graphics/Modules/DX11RenderStageHandler.h"

namespace Sentinel
{
	class Renderer {
	public:
		Renderer(SharedRef<DeviceModules> deviceModules);
		~Renderer();

		void SetPipelineData(SharedRef<PipelineModules> pipelineModules);
		Window& GetWindow();

		void Draw();
		void Clear();
		void Shutdown();
		void Resize(UInt32 width, UInt32 height);

		void FramebufferBind();
		void FramebufferUnbind();

	private:
		SharedRef<DeviceModules> GetDeviceModulesFromRenderData();
		SharedRef<PipelineModules> GetPipelineModulesFromRenderData();

	public:
		UniqueRef<RenderStageHandler> m_RenderStageHandler;
	};
}
