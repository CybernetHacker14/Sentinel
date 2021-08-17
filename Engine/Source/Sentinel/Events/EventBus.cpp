#include "stpch.h"
#include "Sentinel/Events/EventBus.h"

namespace Sentinel
{
	EventBus::EventBus()
		:m_CallbackMapInsertIndex(0) {}

	EventBus::~EventBus() {
		m_EventBus.clear();
		m_CallbackMap.clear();
	}

	const UInt EventBus::SubscribeToEvent(const EventType& eventType, const EventCallbackFn& callback) {
		UInt index = m_CallbackMapInsertIndex;
		m_CallbackMap[eventType].emplace_back(index, callback);
		m_CallbackMapInsertIndex++;
		return index;
	}

	void EventBus::UnsubscribeFromEvent(const EventType& eventType, const UInt& callbackIndex) {
		for (auto it = m_CallbackMap[eventType].begin(); it != m_CallbackMap[eventType].end(); ++it)
		{
			if ((*it).first == callbackIndex)
			{
				m_CallbackMap[eventType].erase(it);
				break;
			}
		}
	}

	void EventBus::NotifyAboutEvent(Scope<Event> eventData) {
		m_EventBus.emplace_back(STL::move(eventData));
	}

	void EventBus::ProcessEvents() {
		if (m_EventBus.empty())
			return;

		for (auto it = m_EventBus.begin(); it != m_EventBus.end();)
		{
			for (auto callIt = m_CallbackMap[(*it)->GetEventType()].begin();
				callIt != m_CallbackMap[(*it)->GetEventType()].end(); ++callIt)
			{
				(*callIt).second(**it);

				if ((*it)->Handled)
					break;
			}

			if ((*it)->Handled)
				m_EventBus.erase(it);

			if (m_EventBus.empty())
				return;
			else
				++it;
		}

		m_EventBus.clear();
	}
}
