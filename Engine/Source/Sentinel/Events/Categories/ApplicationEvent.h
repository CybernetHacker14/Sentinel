//#pragma once
//
//#include "Sentinel/Events/Event.h"
//
//namespace Sentinel
//{
//	class AppTickEvent final : public Event {
//	public:
//		AppTickEvent() = default;
//
//		EventType GetEventType() const final { return EventType::AppTick; }
//		EventCategory GetEventCategoryFlags() const final { return EventCategory::Application; }
//		const char* GetName() const final { return "AppTickEvent"; }
//
//		EVENT_STATIC_TYPE(AppTick)
//	};
//
//	class AppUpdateEvent final : public Event {
//	public:
//		AppUpdateEvent() = default;
//
//		EventType GetEventType() const final { return EventType::AppUpdate; }
//		EventCategory GetEventCategoryFlags() const final { return EventCategory::Application; }
//		const char* GetName() const final { return "AppUpdateEvent"; }
//
//		EVENT_STATIC_TYPE(AppUpdate)
//	};
//
//	class AppRenderEvent final : public Event {
//	public:
//		AppRenderEvent() = default;
//
//		EventType GetEventType() const final { return EventType::AppRender; }
//		EventCategory GetEventCategoryFlags() const final { return EventCategory::Application; }
//		const char* GetName() const final { return "AppRenderEvent"; }
//
//		EVENT_STATIC_TYPE(AppRender)
//	};
//}
