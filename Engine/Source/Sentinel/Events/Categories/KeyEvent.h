#pragma once

#include "Sentinel/Events/Event.h"
#include "Sentinel/Input/KeyCodes.h"

namespace Sentinel
{
	class KeyEvent : public Event {
	public:
		KeyCode GetKeyCode() const { return m_KeyCode; }

		EventCategory GetEventCategoryFlags() const final {
			return EventCategory::Keyboard | EventCategory::Input;
		}

	protected:
		KeyEvent(const KeyCode keycode)
			: m_KeyCode(keycode) {}

		KeyCode m_KeyCode;
	};

	class KeyPressedEvent final : public KeyEvent {
	public:
		KeyPressedEvent(const KeyCode keycode, const UShort repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		UShort GetRepeatCount() const { return m_RepeatCount; }

		EventType GetEventType() const final { return EventType::KeyPressed; }
		const char* GetName() const final { return "KeyPressedEvent"; }

		STL::string ToString() const final {
			std::stringstream stream;
			stream << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return STL::string(stream.str().c_str());
		}

	private:
		UShort m_RepeatCount;
	};

	class KeyReleasedEvent final : public KeyEvent {
	public:
		KeyReleasedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {}

		EventType GetEventType() const final { return EventType::KeyReleased; }
		const char* GetName() const final { return "KeyReleasedEvent"; }

		STL::string ToString() const final {
			std::stringstream stream;
			stream << "KeyReleasedEvent: " << m_KeyCode;
			return STL::string(stream.str().c_str());
		}
	};

	class KeyTypedEvent final : public KeyEvent {
	public:
		KeyTypedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {}

		EventType GetEventType() const final { return EventType::KeyTyped; }
		const char* GetName() const final { return "KeyTypedEvent"; }

		STL::string ToString() const final {
			std::stringstream stream;
			stream << "KeyTypedEvent: " << m_KeyCode;
			return STL::string(stream.str().c_str());
		}
	};
}
