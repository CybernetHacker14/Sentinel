#pragma once

#include "Sentinel/Base/Define.h"
#include "Sentinel/Layers/Layer.h"
#include "Sentinel/Events/Categories/WindowEvent.h"

namespace Sentinel
{
	constexpr STL::string_view WINDOW_CLOSE_EVENT = "Sentinel::WindowCloseEvent";
	constexpr STL::string_view WINDOW_RESIZE_EVENT = "Sentinel::WindowResizeEvent";

	class EventBus {
	public:
		using EventCallbackFn = STL::function<void(Event&)>;

		EventBus() = default;
		~EventBus();

		void SubscribeToEvent(const STL::string& eventCallbackString, const EventCallbackFn& callback);

		void RegisterEvent(Scope<Event> eventData);

		void ProcessEventsImmediate();
		void ProcessEventsImmediate(const Layer& layer);

		void ProcessEventsDeferred();
		void ProcessEventsDeferred(const Layer& layer);

	private:
		STL::vector<Scope<Event>> m_EventBus;

		// If EASTL is being used, STL::vector<const EventCallbackFn> is valid,
		// but standard STL is being used, then it will throw errors

	#if USE_EASTL == 1
		using CallbackMap = STL::unordered_map<STL::string, STL::vector<const EventCallbackFn>>;
	#else
		using CallbackMap = STL::unordered_map<STL::string, STL::vector<EventCallbackFn>>;
	#endif // USE_EASTL == 1

		CallbackMap m_CallbackMap;
	};

	/*class EventDispatcher {
	public:
		EventDispatcher(Event& event)
			:m_Event(event) {}

		template<typename T, typename F>
		bool Dispatch(const F& func) {
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};*/
}
