#pragma once

#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	// This class is actually a management class around
	// an std::vector, which is the actual bus
	class EventBus {
	public:
		EventBus() = default;
		~EventBus();

		void PushEvent(Event* event);
		void PopEvent(Event* event);

		const uint32_t GetSize() const { return static_cast<uint32_t>(m_EventBus.size()); }

		// Iterators

		STL::vector<Event*>::iterator begin() { return m_EventBus.begin(); }
		STL::vector<Event*>::iterator end() { return m_EventBus.end(); }

		// Const iterators

		STL::vector<Event*>::const_iterator begin() const { return m_EventBus.begin(); }
		STL::vector<Event*>::const_iterator end() const { return m_EventBus.end(); }

	private:
		// The actual event bus data structure
		STL::vector<Event*> m_EventBus;
	};
}
