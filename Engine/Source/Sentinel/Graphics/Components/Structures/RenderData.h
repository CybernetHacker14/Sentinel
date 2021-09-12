#pragma once

#include "Sentinel/Base/Define.h"

#include "Sentinel/Window/Window.h"
#include "Sentinel/Graphics/Modules/GraphicsContext.h"

#include "Sentinel/Graphics/Components/Buffers/Vertexbuffer.h"
#include "Sentinel/Graphics/Components/Buffers/Indexbuffer.h"
#include "Sentinel/Graphics/Components/Materials/Shader.h"

#include <glm/glm.hpp>

namespace Sentinel
{
	struct DeviceModules : public IntrusiveRefObject {
	public:
		Scope<WindowProps> WindowProps;
		Scope<Window> Window;
		Scope<GraphicsContextBase> GraphicsContext;
	};

	struct PipelineModules : public IntrusiveRefObject {
	public:
		glm::vec4 ClearColor;
		STL::vector<Ref<VertexbufferBase>> Vertexbuffers;
		Ref<IndexbufferBase> Indexbuffer;
		Ref<ShaderBase> Shader;
	};

	struct RenderData : public IntrusiveRefObject {
	public:
		RenderData() {
			DeviceModules = Sentinel::CreateRef<Sentinel::DeviceModules>();
			PipelineModules = Sentinel::CreateRef<Sentinel::PipelineModules>();
		}

	public:
		Ref<DeviceModules> DeviceModules;
		Ref<PipelineModules> PipelineModules;
	};
}
