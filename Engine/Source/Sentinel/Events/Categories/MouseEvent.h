#pragma once

#include "Sentinel/Events/Event.h"
#include "Sentinel/Input/MouseCodes.h"

namespace Sentinel
{
	class MouseMovedEvent final : public EventCRTP<MouseMovedEvent> {
	public:
		MouseMovedEvent(const Float x, const Float y)
			: m_MouseX(x), m_MouseY(y) {
			Type = EventType::MouseMoved;
		}

		Float GetX() const { return m_MouseX; }
		Float GetY() const { return m_MouseY; }

	private:
		EventCategory GetEventCategoryFlags() const { return EventCategory::Mouse | EventCategory::Input; }
		const char* GetName() const { return "MouseMovedEvent"; }

		STL::string ToString() const {
			std::stringstream stream;
			stream << "MouseMovedEvent: " << m_MouseX << " , " << m_MouseY;
			return STL::string(stream.str().c_str());
		}

	private:
		friend class EventCRTP<MouseMovedEvent>;
		Float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent final : public EventCRTP<MouseScrolledEvent> {
	public:
		MouseScrolledEvent(const Float xOffset, const Float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {
			Type = EventType::MouseScrolled;
		}

		Float GetXOffset() const { return m_XOffset; }
		Float GetYOffset() const { return m_YOffset; }

	private:
		EventCategory GetEventCategoryFlags() const {
			return EventCategory::Mouse | EventCategory::Input;
		}

		const char* GetName() const { return "MouseScrolledEvent"; }

		STL::string ToString() const {
			std::stringstream stream;
			stream << "MouseScrolledEvent: " << m_XOffset << " , " << m_YOffset;
			return STL::string(stream.str().c_str());
		}

	private:
		friend class EventCRTP<MouseScrolledEvent>;
		Float m_XOffset, m_YOffset;
	};

	class MouseButtonPressedEvent final : public EventCRTP<MouseButtonPressedEvent> {
	public:
		MouseButtonPressedEvent(const MouseCode button)
			: m_MouseButton(button) {
			Type = EventType::MouseButtonPressed;
		}

		MouseCode GetMouseCode() const { return m_MouseButton; }

	private:
		EventCategory GetEventCategoryFlags() const {
			return EventCategory::Mouse | EventCategory::Input | EventCategory::MouseButton;
		}

		const char* GetName() const { return "MouseButtonPressedEvent"; }

		STL::string ToString() const {
			std::stringstream stream;
			stream << "MouseButtonPressedEvent: " << m_MouseButton;
			return STL::string(stream.str().c_str());
		}

	private:
		friend class EventCRTP<MouseButtonPressedEvent>;
		MouseCode m_MouseButton;
	};

	class MouseButtonReleasedEvent final : public EventCRTP<MouseButtonReleasedEvent> {
	public:
		MouseButtonReleasedEvent(const MouseCode button)
			: m_MouseButton(button) {
			Type = EventType::MouseButtonReleased;
		}

		MouseCode GetMouseCode() const { return m_MouseButton; }

	private:
		EventCategory GetEventCategoryFlags() const {
			return EventCategory::Mouse | EventCategory::Input | EventCategory::MouseButton;
		}

		const char* GetName() const { return "MouseButtonReleasedEvent"; }

		STL::string ToString() const {
			std::stringstream stream;
			stream << "MouseButtonReleasedEvent: " << m_MouseButton;
			return STL::string(stream.str().c_str());
		}

	private:
		friend class EventCRTP<MouseButtonReleasedEvent>;
		MouseCode m_MouseButton;
	};
}
