#pragma once

#include "Sentinel/Layers/Layer.h"

namespace Sentinel
{
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
		void OnResize(Event& event);

	private:
		UInt32 m_OnResizeCallbackIndex = 0;
	};
}
