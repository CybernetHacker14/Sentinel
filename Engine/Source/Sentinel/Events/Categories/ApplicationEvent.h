#pragma once

#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	class AppTickEvent final : public EventCRTP<AppTickEvent> {
	public:
		AppTickEvent() {
			Type = EventType::AppTick;
		};

	private:
		EventCategory GetEventCategoryFlags() const { return EventCategory::Application; }
		const char* GetName() const { return "AppTickEvent"; }

		STL::string ToString() const {
			return STL::string("AppTickEvent");
		}

	private:
		friend class EventCRTP<AppTickEvent>;
	};

	class AppUpdateEvent final : public EventCRTP<AppUpdateEvent> {
	public:
		AppUpdateEvent() {
			Type = EventType::AppUpdate;
		};

	private:
		EventCategory GetEventCategoryFlags() const { return EventCategory::Application; }
		const char* GetName() const { return "AppUpdateEvent"; }

		STL::string ToString() const {
			return STL::string("AppUpdateEvent");
		}

	private:
		friend class EventCRTP<AppUpdateEvent>;
	};

	class AppRenderEvent final : public EventCRTP<AppRenderEvent> {
	public:
		AppRenderEvent() {
			Type = EventType::AppRender;
		};

	private:
		EventCategory GetEventCategoryFlags() const { return EventCategory::Application; }
		const char* GetName() const { return "AppRenderEvent"; }

		STL::string ToString() const {
			return STL::string("AppRenderEvent");
		}

	private:
		friend class EventCRTP<AppRenderEvent>;
	};
}
