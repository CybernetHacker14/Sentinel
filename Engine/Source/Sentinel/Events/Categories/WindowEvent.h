#pragma once

#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	class WindowCloseEvent final : public Event {
	public:
		WindowCloseEvent() {
			Type = EventType::WindowClose;

			m_GetFlagsFunction = ST_BIND_FN(GetEventCategoryFlags);
			m_GetNameFunction = ST_BIND_FN(GetName);
			m_ToStringFunction = ST_BIND_FN(ToString);
		};

	private:
		inline const EventCategory GetEventCategoryFlags() const { return EventCategory::Application; }
		inline const char* GetName() const { return "WindowCloseEvent"; }
		inline const STL::string ToString() const { return STL::string("WindowCloseEvent"); }
	};

	class WindowResizeEvent final : public Event {
	public:
		WindowResizeEvent(UInt32 width, UInt32 height)
			: m_Width(width), m_Height(height) {
			Type = EventType::WindowResize;

			m_GetFlagsFunction = ST_BIND_FN(GetEventCategoryFlags);
			m_GetNameFunction = ST_BIND_FN(GetName);
			m_ToStringFunction = ST_BIND_FN(ToString);
		}

	public:
		UInt32 GetWidth() const { return m_Width; }
		UInt32 GetHeight() const { return m_Height; }

	private:
		inline const EventCategory GetEventCategoryFlags() const { return EventCategory::Application; }
		inline const char* GetName() const { return "WindowResizeEvent"; }

		inline const STL::string ToString() const {
			std::stringstream stream;
			stream << "WindowsResizeEvent: " << m_Width << " , " << m_Height;
			return STL::string(stream.str().c_str());
		}

	private:
		UInt32 m_Width, m_Height;
	};
}
