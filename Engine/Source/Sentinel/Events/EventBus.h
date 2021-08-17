#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Layers/Layer.h"

namespace Sentinel
{
	class EventBus {
	public:
		using EventCallbackFn = STL::function<void(Event&)>;

		EventBus();
		~EventBus();

	private:
		friend class Application;

		const UInt SubscribeToEvent(const EventType& eventType, const EventCallbackFn& callback);
		void UnsubscribeFromEvent(const EventType& eventType, const UInt& callbackIndex);
		void NotifyAboutEvent(Scope<Event> eventData);

		void ProcessEvents();

	private:
		STL::vector<Scope<Event>> m_EventBus;

		using CallbackMap = STL::unordered_map<EventType, STL::vector<STL::pair<UInt, EventCallbackFn>>>;

		CallbackMap m_CallbackMap;
		UInt m_CallbackMapInsertIndex = 0;
	};
}
