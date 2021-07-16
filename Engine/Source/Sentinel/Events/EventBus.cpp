#include "stpch.h"
#include "Sentinel/Events/EventBus.h"

namespace Sentinel
{
	EventBus::~EventBus() {}

	void EventBus::SubscribeToEvent(const STL::string& eventCallbackString, const EventCallbackFn& callback) {
		m_CallbackMap[eventCallbackString].emplace_back(callback);
	}

	void EventBus::RegisterEvent(Scope<Event> eventData) {
		m_EventBus.emplace_back(STL::move(eventData));
	}

	void EventBus::ProcessEventsImmediate() {
		if (m_EventBus.size() == 0)
			return;

		for (auto it = m_EventBus.begin(); it != m_EventBus.end();)
		{
			switch ((*it)->GetEventType())
			{
				case EventType::WindowClose:
					for (auto callIt = m_CallbackMap[STL::string(WINDOW_CLOSE_EVENT)].begin();
						callIt != m_CallbackMap[STL::string(WINDOW_CLOSE_EVENT)].end(); ++callIt)
					{
						(*callIt)((**it));
					}
					break;
				case EventType::WindowResize:
					for (auto callIt = m_CallbackMap[STL::string(WINDOW_RESIZE_EVENT)].begin();
						callIt != m_CallbackMap[STL::string(WINDOW_RESIZE_EVENT)].end(); ++callIt)
					{
						(*callIt)((**it));
					}
					break;
			}

			if ((*it)->Handled)
				m_EventBus.erase(it);

			if (m_EventBus.size() == 0)
				return;
			else
				++it;
		}
	}

	void EventBus::ProcessEventsImmediate(const Layer& layer) {}

	void EventBus::ProcessEventsDeferred() {}

	void EventBus::ProcessEventsDeferred(const Layer& layer) {}
}
