#pragma once

#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	class WindowCloseEvent final : public Event {
	public:
		WindowCloseEvent() = default;

		EventType GetEventType() const final { return EventType::WindowClose; }
		EventCategory GetEventCategoryFlags() const final { return EventCategory::Application; }
		const char* GetName() const final { return "WindowCloseEvent"; }
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
			std::stringstream stream;
			stream << "WindowsResizeEvent: " << m_Width << " , " << m_Height;
			return STL::string(stream.str().c_str());
		}

	private:
		uint32_t m_Width, m_Height;
	};
}
