#pragma once

#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	/*class WindowCloseEvent final : public BaseEvent<WindowCloseEvent> {
	public:
		using BaseEvent::BaseEvent;

		inline EventType GetEventType() const {
			return EventType::WindowClose;
		}

		inline EventCategory GetEventCategoryFlags() const {
			return EventCategory::Application;
		}

		inline const char* GetName() const {
			return "WindowCloseEvent";
		}

		inline STL::string ToString() const {
			return GetName();
		}

		EVENT_STATIC_TYPE(WindowClose)
	};*/

	/*class WindowResizeEvent final : public BaseEvent<WindowResizeEvent> {
	public:
		using BaseEvent::BaseEvent;

		inline EventType GetEventType() const {
			return EventType::WindowResize;
		}

		inline EventCategory GetEventCategoryFlags() const {
			return EventCategory::Application;
		}

		inline const char* GetName() const {
			return "WindowResizeEvent";
		}

		inline STL::string ToString() const {
			return GetName();
		}

		EVENT_STATIC_TYPE(WindowResize)
	};*/

	class WindowCloseEvent final : public Event {
	public:
		WindowCloseEvent() = default;

		EventType GetEventType() const final { return EventType::WindowClose; }
		EventCategory GetEventCategoryFlags() const final { return EventCategory::Application; }
		const char* GetName() const final { return "WindowCloseEvent"; }

		EVENT_STATIC_TYPE(WindowClose)
	};

	class WindowResizeEvent final : public Event {
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height) {}

		EventType GetEventType() const final { return EventType::WindowResize; }
		EventCategory GetEventCategoryFlags() const final { return EventCategory::Application; }
		const char* GetName() const final { return "WindowResizeEvent"; }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		STL::string ToString() const final {
			STL::string stream;
			stream.reserve(32);
			stream += "WindowsResizeEvent: ";
			stream += m_Width;
			stream += " , ";
			stream += m_Height;
			return stream;
		}

		EVENT_STATIC_TYPE(WindowResize)
	private:
		uint32_t m_Width, m_Height;
	};
}
