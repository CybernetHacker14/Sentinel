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
		WindowResizeEvent(UInt width, UInt height)
			: m_Width(width), m_Height(height) {}

		EventType GetEventType() const final { return EventType::WindowResize; }
		EventCategory GetEventCategoryFlags() const final { return EventCategory::Application; }
		const char* GetName() const final { return "WindowResizeEvent"; }

		UInt GetWidth() const { return m_Width; }
		UInt GetHeight() const { return m_Height; }

		STL::string ToString() const final {
			std::stringstream stream;
			stream << "WindowsResizeEvent: " << m_Width << " , " << m_Height;
			return STL::string(stream.str().c_str());
		}

	private:
		UInt m_Width, m_Height;
	};
}
