#pragma once

#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	// TODO: Design an input system which will define internal Mouse Code data
	// And add that mousecode data to corresponding event classes

	class MouseMovedEvent : public Event {
	public:
		float_t GetX() const { return m_MouseX; }
		float_t GetY() const { return m_MouseY; }

		EventType GetEventType() const override { return EventType::MouseMoved; }
		EventCategory GetEventCategoryFlags() const override { return EventCategory::Mouse | EventCategory::Input; }
		const char* GetName() const override { return "MouseMovedEvent"; }

		STL::string ToString() const override {
			std::stringstream stream;
			stream << "MouseMovedEvent: " << m_MouseX << " , " << m_MouseY;
			return ss.str();
		}
	private:
		float_t m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event {
	public:
		float_t GetXOffset() const { return m_XOffset; }
		float_t GetYOffset() const { return m_YOffset; }

		EventType GetEventType() const override { return EventType::MouseScrolled; }
		EventCategory GetEventCategoryFlags() const override {
			return EventCategory::Mouse | EventCategory::Input;
		}
		const char* GetName() const override { return "MouseScrolledEvent"; }

		STL::string ToString() const override {
			std::stringstream stream;
			stream << "MouseScrolledEvent: " << m_XOffset << " , " << m_YOffset;
			return stream.str();
		}
	private:
		float_t m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event {
	public:
		EventCategory GetEventCategoryFlags() const override {
			return EventCategory::Mouse | EventCategory::Input | EventCategory::MouseButton;
		}
	};

	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		EventType GetEventType() const override { return EventType::MouseButtonPressed; }

		STL::string ToString() const override {
			std::stringstream stream;
			stream << "MouseButtonPressedEvent: ";
			return stream.str();
		}
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		EventType GetEventType() const override { return EventType::MouseButtonReleased; }

		STL::string ToString() const override {
			std::stringstream stream;
			stream << "MouseButtonReleasedEvent: ";
			return stream.str();
		}
	};
}
