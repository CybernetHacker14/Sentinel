#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Graphics/Core/Backend.h"
#include "Sentinel/Graphics/Modules/RenderStageHandler.h"

#include "Platform/DirectX11/Graphics/Modules/DX11RenderStageHandler.h"

namespace Sentinel
{
	class Renderer {
	public:
		Renderer(Ref<DeviceModules> deviceModules, Ref<PipelineModules> pipelineModules);
		~Renderer();

		Window& GetWindow();

	private:
		template<typename T>
		void Init(Ref<DeviceModules> deviceModules, Ref<PipelineModules> pipelineModules);

		Ref<DeviceModules> GetDeviceModulesFromRenderData() const;
		Ref<PipelineModules> GetPipelineModulesFromRenderData() const;

	public:
		Scope<RenderStageHandlerBase> m_RenderStageHandler;
	};
}
