#pragma once

#include "Sentinel/Events/Event.h"
#include "Sentinel/Input/KeyCodes.h"

namespace Sentinel
{
	class KeyPressedEvent final : public Event {
	public:
		KeyPressedEvent(const KeyCode keycode, const Int16 repeatCount)
			: m_KeyCode(keycode), m_RepeatCount(repeatCount) {
			Type = EventType::KeyPressed;

			m_GetFlagsFunction = ST_BIND_FN(GetEventCategoryFlags);
			m_GetNameFunction = ST_BIND_FN(GetName);
			m_ToStringFunction = ST_BIND_FN(ToString);
		}

		Int16 GetRepeatCount() const { return m_RepeatCount; }
		KeyCode GetKeycode() const { return m_KeyCode; }

	private:
		inline const EventCategory GetEventCategoryFlags() const {
			return EventCategory::Keyboard | EventCategory::Input;
		}

		inline const char* GetName() const { return "KeyPressedEvent"; }

		inline const STL::string ToString() const {
			std::stringstream stream;
			stream << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return STL::string(stream.str().c_str());
		}

	private:
		KeyCode m_KeyCode;
		Int16 m_RepeatCount;
	};

	class KeyReleasedEvent final : public Event {
	public:
		KeyReleasedEvent(const KeyCode keycode)
			: m_KeyCode(keycode) {
			Type = EventType::KeyReleased;

			m_GetFlagsFunction = ST_BIND_FN(GetEventCategoryFlags);
			m_GetNameFunction = ST_BIND_FN(GetName);
			m_ToStringFunction = ST_BIND_FN(ToString);
		}

	private:
		inline const EventCategory GetEventCategoryFlags() const {
			return EventCategory::Keyboard | EventCategory::Input;
		}

		inline const char* GetName() const { return "KeyReleasedEvent"; }

		inline const STL::string ToString() const {
			std::stringstream stream;
			stream << "KeyReleasedEvent: " << m_KeyCode;
			return STL::string(stream.str().c_str());
		}

	private:
		KeyCode m_KeyCode;
	};

	class KeyTypedEvent final : public Event {
	public:
		KeyTypedEvent(const KeyCode keycode)
			: m_KeyCode(keycode) {
			Type = EventType::KeyTyped;

			m_GetFlagsFunction = ST_BIND_FN(GetEventCategoryFlags);
			m_GetNameFunction = ST_BIND_FN(GetName);
			m_ToStringFunction = ST_BIND_FN(ToString);
		}

	private:
		inline const EventCategory GetEventCategoryFlags() const {
			return EventCategory::Keyboard | EventCategory::Input;
		}

		inline const char* GetName() const { return "KeyTypedEvent"; }

		inline const STL::string ToString() const {
			std::stringstream stream;
			stream << "KeyTypedEvent: " << m_KeyCode;
			return STL::string(stream.str().c_str());
		}

	private:
		KeyCode m_KeyCode;
	};
}
