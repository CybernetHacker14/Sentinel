#pragma once

#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	class WindowCloseEvent final : public EventCRTP<WindowCloseEvent> {
	public:
		WindowCloseEvent() {
			Type = EventType::WindowClose;
		};

	private:
		EventCategory GetEventCategoryFlags() const { return EventCategory::Application; }
		const char* GetName() const { return "WindowCloseEvent"; }

		STL::string ToString() const {
			return STL::string("WindowCloseEvent");
		}

	private:
		friend class EventCRTP<WindowCloseEvent>;
	};

	class WindowResizeEvent final : public EventCRTP<WindowResizeEvent> {
	public:
		WindowResizeEvent(UInt32 width, UInt32 height)
			: m_Width(width), m_Height(height) {
			Type = EventType::WindowResize;
		}

	public:
		UInt32 GetWidth() const { return m_Width; }
		UInt32 GetHeight() const { return m_Height; }

	private:
		EventCategory GetEventCategoryFlags() const { return EventCategory::Application; }
		const char* GetName() const { return "WindowResizeEvent"; }

		STL::string ToString() const {
			std::stringstream stream;
			stream << "WindowsResizeEvent: " << m_Width << " , " << m_Height;
			return STL::string(stream.str().c_str());
		}

	private:
		friend class EventCRTP<WindowResizeEvent>;
		UInt32 m_Width, m_Height;
	};
}
