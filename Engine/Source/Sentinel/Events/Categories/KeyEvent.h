#pragma once

#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	// TODO: Design an input system which will define internal Key Code data
	// And add that keycode data to corresponding event classes

	class KeyEvent : public Event {
	protected:
		KeyEvent() = default;

		EventCategory GetEventCategoryFlags() const override {
			return EventCategory::Keyboard | EventCategory::Input;
		}
	};

	class KeyPressedEvent : public KeyEvent {
	public:
		uint16_t GetRepeatCount() const { return m_RepeatCount; }

		EventType GetEventType() const override { return EventType::KeyPressed; }
		const char* GetName() const override { return "KeyPressedEvent"; }

		STL::string ToString() const override {
			std::stringstream stream;
			stream << "KeyPressedEvent: " << " (" << m_RepeatCount << " repeats";
			return stream.str();
		}

	private:
		uint16_t m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent {
	public:
		EventType GetEventType() const override { return EventType::KeyReleased; }
		const char* GetName() const override { return "KeyReleasedEvent"; }

		STL::string ToString() const override {
			std::stringstream stream;
			stream << "KeyReleasedEvent: ";
			return stream.str();
		}
	};

	class KeyTypedEvent : public KeyEvent {
	public:
		EventType GetEventType() const override { return EventType::KeyTyped; }
		const char* GetName() const override { return "KeyTypedEvent"; }

		STL::string ToString() const override {
			std::stringstream stream;
			stream << "KeyTypedEvent: ";
			return stream.str();
		}
	};
}
