#include "stpch.h"
#include "Sentinel/Events/EventBus.h"

namespace Sentinel
{
	EventBus::~EventBus() {}

	void EventBus::SubscribeToEvent(const EventType& eventType, const EventCallbackFn& callback) {
		m_CallbackMap[eventType].emplace_back(callback);
	}

	void EventBus::NotifyAboutEvent(Scope<Event> eventData) {
		m_EventBus.emplace_back(STL::move(eventData));
	}

	void EventBus::ProcessEventsImmediate() {
		ProcessEvents(s_ImmediateFilter);
	}

	void EventBus::ProcessEventsDeferred() {
		ProcessEvents(s_DeferredFilter);
	}

	void EventBus::ProcessEvents(const STL::vector<EventType>& typeFilter) {
		if (m_EventBus.size() == 0)
			return;

		for (auto it = m_EventBus.begin(); it != m_EventBus.end();)
		{
			if (STL::find(typeFilter.begin(), typeFilter.end(), (*it)->GetEventType()) == typeFilter.end())
				continue;

			for (auto callIt = m_CallbackMap[(*it)->GetEventType()].begin();
				callIt != m_CallbackMap[(*it)->GetEventType()].end(); ++callIt)
			{
				(*callIt)((**it));
			}

			if ((*it)->Handled)
				m_EventBus.erase(it);

			if (m_EventBus.size() == 0)
				return;
			else
				++it;
		}
	}
}
