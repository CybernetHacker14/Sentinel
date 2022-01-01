#pragma once

#include "Sentinel/Common/Common.h"

#include "Sentinel/Window/Window.h"
#include "Sentinel/Graphics/Modules/GraphicsContext.h"

#include "Sentinel/Graphics/Components/Buffers/Vertexbuffer.h"
#include "Sentinel/Graphics/Components/Buffers/Indexbuffer.h"
#include "Sentinel/Graphics/Components/Buffers/Framebuffer.h"
#include "Sentinel/Graphics/Components/Materials/Shader.h"

#include "Sentinel/Graphics/Components/Materials/Texture2D.h"

#include <glm/glm.hpp>

namespace Sentinel
{
	struct DeviceModules : public ISharedRef {
	public:
		UniqueRef<WindowProps> WindowProps;
		UniqueRef<Window> Window;
		UniqueRef<GraphicsContext> GraphicsContext;
	};

	struct PipelineModules : public ISharedRef {
	public:
		SharedRef<Framebuffer> Framebuffer;
		STL::vector<SharedRef<Vertexbuffer>> Vertexbuffers;
		SharedRef<Indexbuffer> Indexbuffer;
		SharedRef<Shader> Shader;
	};

	struct RenderData : public ISharedRef {
	public:
		RenderData() {
			DeviceModules = CreateSharedRef<Sentinel::DeviceModules>();
			PipelineModules = CreateSharedRef<Sentinel::PipelineModules>();
		}

	public:
		SharedRef<DeviceModules> DeviceModules;
		SharedRef<PipelineModules> PipelineModules;
	};
}
