#pragma once

#include "Sentinel/Base/Define.h"

namespace Sentinel
{
	// Enum for depicting the type of event
	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// Enum for depicting the category of event
	enum class EventCategory {
		None = 0,
		Application = BIT(0),
		Input = BIT(1),
		Keyboard = BIT(2),
		Mouse = BIT(3),
		MouseButton = BIT(4)
	};

	// Bitwise operations for enum EventCategory

	EventCategory operator |(EventCategory lhs, EventCategory rhs);
	EventCategory operator &(EventCategory lhs, EventCategory rhs);
	EventCategory operator ^(EventCategory lhs, EventCategory rhs);
	EventCategory operator ~(EventCategory category);
	EventCategory& operator |=(EventCategory& lhs, EventCategory rhs);
	EventCategory& operator &=(EventCategory& lhs, EventCategory rhs);
	EventCategory& operator ^=(EventCategory& lhs, EventCategory rhs);

	//#define EVENT_STATIC_TYPE(type) static EventType GetStaticType() { return EventType::type; }

	//// A pure virtual Event class.
	//// Derived classes from Event will store event data for their respective events
	class Event {
	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual EventCategory GetEventCategoryFlags() const = 0;
		virtual const char* GetName() const = 0;
		virtual STL::string ToString() const { return GetName(); }

		Bool IsInCategory(EventCategory category) {
			return static_cast<Bool>(GetEventCategoryFlags() & category);
		}
	public:
		Bool Handled = false;
	};
}
