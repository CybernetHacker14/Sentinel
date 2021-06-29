#pragma once

#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	class AppTickEvent : public Event {
	public:
		AppTickEvent() = default;

		EventType GetEventType() const override { return EventType::AppTick; }
		EventCategory GetEventCategoryFlags() const override { return EventCategory::Application; }
		const char* GetName() const override { return "AppTickEvent"; }
	};

	class AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() = default;

		EventType GetEventType() const override { return EventType::AppUpdate; }
		EventCategory GetEventCategoryFlags() const override { return EventCategory::Application; }
		const char* GetName() const override { return "AppUpdateEvent"; }
	};

	class AppRenderEvent : public Event {
	public:
		AppRenderEvent() = default;

		EventType GetEventType() const override { return EventType::AppRender; }
		EventCategory GetEventCategoryFlags() const override { return EventCategory::Application; }
		const char* GetName() const override { return "AppRenderEvent"; }
	};
}
