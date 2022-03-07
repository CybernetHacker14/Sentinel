#pragma once

#include "Sentinel/Events/Event.h"
#include "Sentinel/Input/MouseCodes.h"

namespace Sentinel
{
	class MouseMovedEvent final : public Event {
	public:
		MouseMovedEvent(const Float x, const Float y)
			: m_MouseX(x), m_MouseY(y) {
			Type = EventType::MouseMoved;

			m_GetEventCategoryFlagsFunction = ST_BIND_EVENT_FN(GetEventCategoryFlags);
			m_GetNameFunction = ST_BIND_EVENT_FN(GetName);
			m_ToStringFunction = ST_BIND_EVENT_FN(ToString);
		}

		Float GetX() const { return m_MouseX; }
		Float GetY() const { return m_MouseY; }

	private:
		inline const EventCategory GetEventCategoryFlags() const { return EventCategory::Mouse | EventCategory::Input; }
		inline const char* GetName() const { return "MouseMovedEvent"; }

		inline const STL::string ToString() const {
			std::stringstream stream;
			stream << "MouseMovedEvent: " << m_MouseX << " , " << m_MouseY;
			return STL::string(stream.str().c_str());
		}

	private:
		Float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent final : public Event {
	public:
		MouseScrolledEvent(const Float xOffset, const Float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {
			Type = EventType::MouseScrolled;

			m_GetEventCategoryFlagsFunction = ST_BIND_EVENT_FN(GetEventCategoryFlags);
			m_GetNameFunction = ST_BIND_EVENT_FN(GetName);
			m_ToStringFunction = ST_BIND_EVENT_FN(ToString);
		}

		Float GetXOffset() const { return m_XOffset; }
		Float GetYOffset() const { return m_YOffset; }

	private:
		inline const EventCategory GetEventCategoryFlags() const {
			return EventCategory::Mouse | EventCategory::Input;
		}

		inline const char* GetName() const { return "MouseScrolledEvent"; }

		inline const STL::string ToString() const {
			std::stringstream stream;
			stream << "MouseScrolledEvent: " << m_XOffset << " , " << m_YOffset;
			return STL::string(stream.str().c_str());
		}

	private:
		Float m_XOffset, m_YOffset;
	};

	class MouseButtonPressedEvent final : public Event {
	public:
		MouseButtonPressedEvent(const MouseCode button)
			: m_MouseButton(button) {
			Type = EventType::MouseButtonPressed;

			m_GetEventCategoryFlagsFunction = ST_BIND_EVENT_FN(GetEventCategoryFlags);
			m_GetNameFunction = ST_BIND_EVENT_FN(GetName);
			m_ToStringFunction = ST_BIND_EVENT_FN(ToString);
		}

		MouseCode GetMouseCode() const { return m_MouseButton; }

	private:
		inline const EventCategory GetEventCategoryFlags() const {
			return EventCategory::Mouse | EventCategory::Input | EventCategory::MouseButton;
		}

		inline const char* GetName() const { return "MouseButtonPressedEvent"; }

		inline const STL::string ToString() const {
			std::stringstream stream;
			stream << "MouseButtonPressedEvent: " << m_MouseButton;
			return STL::string(stream.str().c_str());
		}

	private:
		MouseCode m_MouseButton;
	};

	class MouseButtonReleasedEvent final : public Event {
	public:
		MouseButtonReleasedEvent(const MouseCode button)
			: m_MouseButton(button) {
			Type = EventType::MouseButtonReleased;

			m_GetEventCategoryFlagsFunction = ST_BIND_EVENT_FN(GetEventCategoryFlags);
			m_GetNameFunction = ST_BIND_EVENT_FN(GetName);
			m_ToStringFunction = ST_BIND_EVENT_FN(ToString);
		}

		MouseCode GetMouseCode() const { return m_MouseButton; }

	private:
		inline const EventCategory GetEventCategoryFlags() const {
			return EventCategory::Mouse | EventCategory::Input | EventCategory::MouseButton;
		}

		inline const char* GetName() const { return "MouseButtonReleasedEvent"; }

		inline const STL::string ToString() const {
			std::stringstream stream;
			stream << "MouseButtonReleasedEvent: " << m_MouseButton;
			return STL::string(stream.str().c_str());
		}

	private:
		MouseCode m_MouseButton;
	};
}
