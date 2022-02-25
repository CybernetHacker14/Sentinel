#pragma once

#include "Sentinel/Layers/Layer.h"
#include "Sentinel/Graphics/Components/Cameras/Camera.h"

namespace Sentinel
{
	class ImGuiDebugLayer : public Layer {
	public:
		ImGuiDebugLayer(SharedRef<Camera> camera);
		~ImGuiDebugLayer() = default;

		virtual void OnImGuiRender() override;

	private:
		SharedRef<Camera> m_Camera;
	};
}
