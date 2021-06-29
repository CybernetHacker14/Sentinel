#include "stpch.h"
#include "Sentinel/Layers/LayerStack.h"

namespace Sentinel
{
	LayerStack::~LayerStack() {
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto iterator = STL::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
		if (iterator != m_Layers.begin() + m_LayerInsertIndex)
		{
			layer->OnDetach();
			m_Layers.erase(iterator);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::PopOverlay(Layer* overlay) {
		auto iterator = STL::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
		if (iterator != m_Layers.end())
		{
			overlay->OnDetach();
			m_Layers.erase(iterator);
		}
	}
}
