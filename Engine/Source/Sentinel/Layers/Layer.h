#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	class Layer {
	public:
		Layer(const STL::string& debugName = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnRender() {};
		virtual void OnImGuiRender() {};

		const STL::string& GetName() const { return m_DebugName; }
	private:
		STL::string m_DebugName;
	};
}
