#pragma once

#include "Sentinel/Common/Common.h"
#include "Sentinel/Layers/Layer.h"

namespace Sentinel
{
	// A vector of layers. Layers are stored here and then
	// evaluated in the order in which they are stored.
	class LayerStack {
	public:
		LayerStack() = default;
		~LayerStack();

		// There would be two types of layers -
		// normal layers & overlays
		// The overlays would be on top of all normal layers,
		// hence they are at the last of the vector

		// TODO : Discuss whether it would be better have Overlays as a separate class,
		// inherited from base Layer class

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		void CleanLayerstack();

		const UInt32 GetSize() const { return static_cast<UInt32>(m_Layers.size()); }

		// Iterators

		STL::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		STL::vector<Layer*>::iterator end() { return m_Layers.end(); }
		STL::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		STL::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

		// Const Iterators

		STL::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		STL::vector<Layer*>::const_iterator end() const { return m_Layers.end(); }
		STL::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		STL::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }

	private:
		STL::vector<Layer*> m_Layers;
		UInt32 m_LayerInsertIndex = 0;
	};
}
