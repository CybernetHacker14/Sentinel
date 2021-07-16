//#pragma once
//
//#include "Sentinel/Events/Event.h"
//#include "Sentinel/Input/MouseCodes.h"
//
//namespace Sentinel
//{
//	class MouseMovedEvent final : public Event {
//	public:
//		MouseMovedEvent(const float_t x, const float_t y)
//			: m_MouseX(x), m_MouseY(y) {}
//
//		float_t GetX() const { return m_MouseX; }
//		float_t GetY() const { return m_MouseY; }
//
//		EventType GetEventType() const final { return EventType::MouseMoved; }
//		EventCategory GetEventCategoryFlags() const final { return EventCategory::Mouse | EventCategory::Input; }
//		const char* GetName() const final { return "MouseMovedEvent"; }
//
//		STL::string ToString() const final {
//			STL::string stream;
//			stream.reserve(28);
//			stream += "MouseMovedEvent: ";
//			stream += m_MouseX;
//			stream += " , ";
//			stream += m_MouseY;
//			return stream;
//		}
//
//		EVENT_STATIC_TYPE(MouseMoved)
//	private:
//		float_t m_MouseX, m_MouseY;
//	};
//
//	class MouseScrolledEvent final : public Event {
//	public:
//		MouseScrolledEvent(const float_t xOffset, const float_t yOffset)
//			: m_XOffset(xOffset), m_YOffset(yOffset) {}
//
//		float_t GetXOffset() const { return m_XOffset; }
//		float_t GetYOffset() const { return m_YOffset; }
//
//		EventType GetEventType() const final { return EventType::MouseScrolled; }
//		EventCategory GetEventCategoryFlags() const final {
//			return EventCategory::Mouse | EventCategory::Input;
//		}
//		const char* GetName() const final { return "MouseScrolledEvent"; }
//
//		STL::string ToString() const final {
//			STL::string stream;
//			stream.reserve(31);
//			stream += "MouseScrolledEvent: ";
//			stream += m_XOffset;
//			stream += " , ";
//			stream += m_YOffset;
//			return stream;
//		}
//
//		EVENT_STATIC_TYPE(MouseScrolled)
//	private:
//		float_t m_XOffset, m_YOffset;
//	};
//
//	class MouseButtonEvent : public Event {
//	public:
//		MouseCode GetMouseCode() const { return m_MouseButton; }
//
//		EventCategory GetEventCategoryFlags() const final {
//			return EventCategory::Mouse | EventCategory::Input | EventCategory::MouseButton;
//		}
//
//	protected:
//		MouseButtonEvent(const MouseCode button)
//			: m_MouseButton(button) {}
//
//		MouseCode m_MouseButton;
//	};
//
//	class MouseButtonPressedEvent final : public MouseButtonEvent {
//	public:
//		MouseButtonPressedEvent(const MouseCode button)
//			: MouseButtonEvent(button) {}
//
//		EventType GetEventType() const final { return EventType::MouseButtonPressed; }
//
//		STL::string ToString() const final {
//			STL::string stream;
//			stream.reserve(27);
//			stream += "MouseButtonPressedEvent: ";
//			stream += m_MouseButton;
//			return stream;
//		}
//
//		EVENT_STATIC_TYPE(MouseButtonPressed)
//	};
//
//	class MouseButtonReleasedEvent final : public MouseButtonEvent {
//	public:
//		MouseButtonReleasedEvent(const MouseCode button)
//			: MouseButtonEvent(button) {}
//
//		EventType GetEventType() const final { return EventType::MouseButtonReleased; }
//
//		STL::string ToString() const final {
//			STL::string stream;
//			stream.reserve(28);
//			stream += "MouseButtonReleasedEvent: ";
//			stream += m_MouseButton;
//			return stream;
//		}
//
//		EVENT_STATIC_TYPE(MouseButtonReleased)
//	};
//}
