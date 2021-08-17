#pragma once

#include "Sentinel/Events/Event.h"
#include "Sentinel/Input/MouseCodes.h"

namespace Sentinel
{
	class MouseMovedEvent final : public Event {
	public:
		MouseMovedEvent(const Float x, const Float y)
			: m_MouseX(x), m_MouseY(y) {}

		Float GetX() const { return m_MouseX; }
		Float GetY() const { return m_MouseY; }

		EventType GetEventType() const final { return EventType::MouseMoved; }
		EventCategory GetEventCategoryFlags() const final { return EventCategory::Mouse | EventCategory::Input; }
		const char* GetName() const final { return "MouseMovedEvent"; }

		STL::string ToString() const final {
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
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		Float GetXOffset() const { return m_XOffset; }
		Float GetYOffset() const { return m_YOffset; }

		EventType GetEventType() const final { return EventType::MouseScrolled; }
		EventCategory GetEventCategoryFlags() const final {
			return EventCategory::Mouse | EventCategory::Input;
		}
		const char* GetName() const final { return "MouseScrolledEvent"; }

		STL::string ToString() const final {
			std::stringstream stream;
			stream << "MouseScrolledEvent: " << m_XOffset << " , " << m_YOffset;
			return STL::string(stream.str().c_str());
		}

	private:
		Float m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event {
	public:
		MouseCode GetMouseCode() const { return m_MouseButton; }

		EventCategory GetEventCategoryFlags() const final {
			return EventCategory::Mouse | EventCategory::Input | EventCategory::MouseButton;
		}

	protected:
		MouseButtonEvent(const MouseCode button)
			: m_MouseButton(button) {}

		MouseCode m_MouseButton;
	};

	class MouseButtonPressedEvent final : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		EventType GetEventType() const final { return EventType::MouseButtonPressed; }

		const char* GetName() const final { return "MouseButtonPressedEvent"; }

		STL::string ToString() const final {
			std::stringstream stream;
			stream << "MouseButtonPressedEvent: " << m_MouseButton;
			return STL::string(stream.str().c_str());
		}
	};

	class MouseButtonReleasedEvent final : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		EventType GetEventType() const final { return EventType::MouseButtonReleased; }

		const char* GetName() const final { return "MouseButtonReleasedEvent"; }

		STL::string ToString() const final {
			std::stringstream stream;
			stream << "MouseButtonReleasedEvent: " << m_MouseButton;
			return STL::string(stream.str().c_str());
		}
	};
}
