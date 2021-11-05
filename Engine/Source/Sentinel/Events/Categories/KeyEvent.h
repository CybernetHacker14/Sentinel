#pragma once

#include "Sentinel/Events/Event.h"
#include "Sentinel/Input/KeyCodes.h"

namespace Sentinel
{
	class KeyPressedEvent final : public EventCRTP<KeyPressedEvent> {
	public:
		KeyPressedEvent(const KeyCode keycode, const Int16 repeatCount)
			: m_KeyCode(keycode), m_RepeatCount(repeatCount) {
			Type = EventType::KeyPressed;
		}

		Int16 GetRepeatCount() const { return m_RepeatCount; }

	private:
		EventCategory GetEventCategoryFlags() const {
			return EventCategory::Keyboard | EventCategory::Input;
		}

		const char* GetName() const { return "KeyPressedEvent"; }

		STL::string ToString() const {
			std::stringstream stream;
			stream << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return STL::string(stream.str().c_str());
		}

	private:
		friend class EventCRTP<KeyPressedEvent>;
		KeyCode m_KeyCode;
		Int16 m_RepeatCount;
	};

	class KeyReleasedEvent final : public EventCRTP<KeyReleasedEvent> {
	public:
		KeyReleasedEvent(const KeyCode keycode)
			: m_KeyCode(keycode) {
			Type = EventType::KeyReleased;
		}

	private:
		EventCategory GetEventCategoryFlags() const {
			return EventCategory::Keyboard | EventCategory::Input;
		}

		const char* GetName() const { return "KeyReleasedEvent"; }

		STL::string ToString() const {
			std::stringstream stream;
			stream << "KeyReleasedEvent: " << m_KeyCode;
			return STL::string(stream.str().c_str());
		}

	private:
		friend class EventCRTP<KeyReleasedEvent>;
		KeyCode m_KeyCode;
	};

	class KeyTypedEvent final : public EventCRTP<KeyTypedEvent> {
	public:
		KeyTypedEvent(const KeyCode keycode)
			: m_KeyCode(keycode) {
			Type = EventType::KeyTyped;
		}

	private:
		EventCategory GetEventCategoryFlags() const {
			return EventCategory::Keyboard | EventCategory::Input;
		}

		const char* GetName() const { return "KeyTypedEvent"; }

		STL::string ToString() const {
			std::stringstream stream;
			stream << "KeyTypedEvent: " << m_KeyCode;
			return STL::string(stream.str().c_str());
		}

	private:
		friend class EventCRTP<KeyTypedEvent>;
		KeyCode m_KeyCode;
	};
}
