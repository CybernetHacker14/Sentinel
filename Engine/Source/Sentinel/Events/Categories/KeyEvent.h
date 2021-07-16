//#pragma once
//
//#include "Sentinel/Events/Event.h"
//#include "Sentinel/Input/KeyCodes.h"
//
//namespace Sentinel
//{
//	class KeyEvent : public Event {
//	public:
//		public KeyCode GetKeyCode() const { return m_KeyCode; }
//
//		EventCategory GetEventCategoryFlags() const final {
//			return EventCategory::Keyboard | EventCategory::Input;
//		}
//
//	protected:
//		KeyEvent(const KeyCode keycode)
//			: m_KeyCode(keycode) {}
//
//		KeyCode m_KeyCode;
//	};
//
//	class KeyPressedEvent final : public KeyEvent {
//	public:
//		KeyPressedEvent(const KeyCode keycode, const uint16_t repeatCount)
//			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}
//
//		uint16_t GetRepeatCount() const { return m_RepeatCount; }
//
//		EventType GetEventType() const final { return EventType::KeyPressed; }
//		const char* GetName() const final { return "KeyPressedEvent"; }
//
//		STL::string ToString() const final {
//			STL::string stream;
//			stream += "KeyPressedEvent: ";
//			stream += m_KeyCode;
//			stream += " (";
//			stream += m_RepeatCount;
//			stream += " repeats)";
//			return stream;
//		}
//
//		EVENT_STATIC_TYPE(KeyPressed)
//	private:
//		uint16_t m_RepeatCount;
//	};
//
//	class KeyReleasedEvent final : public KeyEvent {
//	public:
//		KeyReleasedEvent(const KeyCode keycode)
//			: KeyEvent(keycode) {}
//
//		EventType GetEventType() const final { return EventType::KeyReleased; }
//		const char* GetName() const final { return "KeyReleasedEvent"; }
//
//		STL::string ToString() const final {
//			STL::string stream;
//			stream += "KeyReleasedEvent: ";
//			stream += m_KeyCode;
//			return stream;
//		}
//
//		EVENT_STATIC_TYPE(KeyReleased)
//	};
//
//	class KeyTypedEvent final : public KeyEvent {
//	public:
//		KeyTypedEvent(const KeyCode keycode)
//			: KeyEvent(keycode) {}
//
//		EventType GetEventType() const final { return EventType::KeyTyped; }
//		const char* GetName() const final { return "KeyTypedEvent"; }
//
//		STL::string ToString() const final {
//			STL::string stream;
//			stream += "KeyTypedEvent: ";
//			stream += m_KeyCode;
//			return stream;
//		}
//
//		EVENT_STATIC_TYPE(KeyTyped)
//	};
//}
