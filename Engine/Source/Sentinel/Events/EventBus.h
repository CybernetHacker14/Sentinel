#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Layers/Layer.h"
#include "Sentinel/Events/Categories/WindowEvent.h"

namespace Sentinel
{
	class EventBus {
	public:
		inline static const STL::vector<EventType> s_DeferredFilter = { };
		inline static const STL::vector<EventType> s_ImmediateFilter = { EventType::WindowClose, EventType::WindowResize };
	public:
		using EventCallbackFn = STL::function<void(Event&)>;

		EventBus() = default;
		~EventBus();

		void ProcessEventsImmediate();
		void ProcessEventsDeferred();
	private:
		friend class Application;

		void SubscribeToEvent(const EventType& eventType, const EventCallbackFn& callback);
		//void UnsubscribeFromEvent(const EventType& eventType, const EventCallbackFn& callback);
		void NotifyAboutEvent(Scope<Event> eventData);

		void ProcessEvents(const STL::vector<EventType>& typeFilter);

	private:
		STL::vector<Scope<Event>> m_EventBus;

		// If EASTL is being used, STL::vector<const EventCallbackFn> is valid,
		// but standard STL is being used, then it will throw errors

	#if USE_EASTL == 1
		using CallbackMap = STL::unordered_map<EventType, STL::vector<const EventCallbackFn>>;
	#else
		using CallbackMap = STL::unordered_map<STL::string, STL::vector<EventCallbackFn>>;
	#endif // USE_EASTL == 1

		CallbackMap m_CallbackMap;
	};
	}
