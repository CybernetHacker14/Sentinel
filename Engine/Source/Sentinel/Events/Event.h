#pragma once

#include "Sentinel/Common/Common.h"

#include <sstream>

namespace Sentinel
{
	template<typename T>
	class EventCRTP;

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

	class Event {
	public:
		EventCategory GetEventCategoryFlags();
		const char* GetName();
		const STL::string& ToString();

		EventType GetEventType() { return Type; }

		Bool IsInCategory(EventCategory category) {
			return static_cast<Bool>(GetEventCategoryFlags() & category);
		}

	public:
		Bool Handled = false;

	protected:
		Event() = default;

		EventType Type;

	public:
		template<typename T>
		inline T* DerivedDowncast() {
			static_assert(STL::is_base_of<EventCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from EventCRTP<T>.");
			return static_cast<T*>(this);
		}

	private:
		template<typename T>
		inline EventCRTP<T>* BaseDowncast() {
			static_assert(STL::is_base_of<EventCRTP<T>, T>::value,
				"Operation not allowed. 'T' should be a derived from EventCRTP<T>.");
			return static_cast<EventCRTP<T>*>(this);
		}
	};

	template<typename T>
	class EventCRTP : public Event {
	private:
		inline EventCategory GetEventCategoryFlags() {
			return underlying().GetEventCategoryFlags();
		}

		inline const char* GetName() {
			return underlying().GetName();
		}

		inline const STL::string& ToString() {
			return underlying().ToString();
		}

	private:
		friend T;
		friend Event;
		EventCRTP() = default;

		inline T& underlying() {
			return static_cast<T&>(*this);
		}
	};
}
