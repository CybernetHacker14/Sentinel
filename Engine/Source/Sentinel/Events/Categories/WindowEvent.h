#pragma once

#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() = default;

		EventType GetEventType() const override { return EventType::WindowClose; }
		EventCategory GetEventCategoryFlags() const override { return EventCategory::Application; }
		const char* GetName() const override { return "WindowCloseEvent"; }
	};

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height) {}

		EventType GetEventType() const override { return EventType::WindowResize; }
		EventCategory GetEventCategoryFlags() const override { return EventCategory::Application; }
		const char* GetName() const override { return "WindowResizeEvent"; }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		STL::string ToString() const override {
			std::stringstream stream;
			stream << "WindowResizeEvent: " << m_Width << " , " << m_Height;
			return stream.str();
		}

	private:
		uint32_t m_Width, m_Height;
	};
}
