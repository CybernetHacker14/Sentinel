#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	class EventBus {
	public:
		using EventCallbackFn = STL::function<void(Event&)>;

		EventBus();
		~EventBus();

	private:
		friend class Application;

		const UInt32 SubscribeToEvent(const EventType& eventType, const EventCallbackFn& callback);
		void UnsubscribeFromEvent(const EventType& eventType, const UInt32& callbackIndex);
		void NotifyAboutEvent(Scope<Event> eventData);

		void ProcessEvents();

	private:
		STL::vector<Scope<Event>> m_EventBus;

		using CallbackMap = STL::unordered_map<EventType, STL::vector<STL::pair<UInt32, EventCallbackFn>>>;

		CallbackMap m_CallbackMap;
		UInt32 m_CallbackMapInsertIndex = 0;
	};
}
