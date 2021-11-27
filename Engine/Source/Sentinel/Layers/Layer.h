#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	class Layer {
	public:
		Layer(const STL::string& debugName = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnImGuiRender() = 0;

		const STL::string& GetName() const { return m_DebugName; }
	private:
		STL::string m_DebugName;
	};
}
