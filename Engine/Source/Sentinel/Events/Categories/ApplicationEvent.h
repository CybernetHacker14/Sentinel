#pragma once

#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	class AppTickEvent final : public Event {
	public:
		AppTickEvent() {
			Type = EventType::AppTick;

			m_GetEventCategoryFlagsFunction = ST_BIND_EVENT_FN(GetEventCategoryFlags);
			m_GetNameFunction = ST_BIND_EVENT_FN(GetName);
			m_ToStringFunction = ST_BIND_EVENT_FN(ToString);
		};

	private:
		inline const EventCategory GetEventCategoryFlags() const { return EventCategory::Application; }
		inline const char* GetName() const { return "AppTickEvent"; }
		inline const STL::string ToString() const { return STL::string("AppTickEvent"); }
	};

	class AppUpdateEvent final : public Event {
	public:
		AppUpdateEvent() {
			Type = EventType::AppUpdate;

			m_GetEventCategoryFlagsFunction = ST_BIND_EVENT_FN(GetEventCategoryFlags);
			m_GetNameFunction = ST_BIND_EVENT_FN(GetName);
			m_ToStringFunction = ST_BIND_EVENT_FN(ToString);
		};

	private:
		inline const EventCategory GetEventCategoryFlags() const { return EventCategory::Application; }
		inline const char* GetName() const { return "AppUpdateEvent"; }
		inline const STL::string ToString() const { return STL::string("AppUpdateEvent"); }
	};

	class AppRenderEvent final : public Event {
	public:
		AppRenderEvent() {
			Type = EventType::AppRender;

			m_GetEventCategoryFlagsFunction = ST_BIND_EVENT_FN(GetEventCategoryFlags);
			m_GetNameFunction = ST_BIND_EVENT_FN(GetName);
			m_ToStringFunction = ST_BIND_EVENT_FN(ToString);
		};

	private:
		inline const EventCategory GetEventCategoryFlags() const { return EventCategory::Application; }
		inline const char* GetName() const { return "AppRenderEvent"; }
		inline const STL::string ToString() const { return STL::string("AppRenderEvent"); }
	};
}
