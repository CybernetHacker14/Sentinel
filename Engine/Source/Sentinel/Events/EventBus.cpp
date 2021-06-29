#include "stpch.h"
#include "Sentinel/Events/EventBus.h"

namespace Sentinel
{
	EventBus::~EventBus() {
		for (Event* event : m_EventBus)
		{
			delete event;
		}
	}

	void EventBus::PushEvent(Event* event) {
		m_EventBus.emplace_back(event);
	}

	void EventBus::PopEvent(Event* event) {
		auto iterator = STL::find(m_EventBus.begin(), m_EventBus.end(), event);
		if (iterator != m_EventBus.end())
		{
			m_EventBus.erase(iterator);
		}
	}
}
