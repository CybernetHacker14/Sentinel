#include "stpch.h"
#include "Sentinel/Layers/Layer.h"

namespace Sentinel
{
	Layer::Layer(const STL::string& debugName)
		:m_DebugName(debugName) {}

	Layer::Layer(const char* debugName)
		: m_DebugName(debugName) {}
}
