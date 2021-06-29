#include "stpch.h"
#include "Sentinel/Events/Event.h"

namespace Sentinel
{
	EventCategory operator|(EventCategory lhs, EventCategory rhs) {
		return static_cast<EventCategory>(
			static_cast<STL::underlying_type_t<EventCategory>>(lhs) |
			static_cast<STL::underlying_type_t<EventCategory>>(rhs)
			);
	}

	EventCategory operator&(EventCategory lhs, EventCategory rhs) {
		return static_cast<EventCategory>(
			static_cast<STL::underlying_type_t<EventCategory>>(lhs) &
			static_cast<STL::underlying_type_t<EventCategory>>(rhs)
			);
	}

	EventCategory operator^(EventCategory lhs, EventCategory rhs) {
		return static_cast<EventCategory>(
			static_cast<STL::underlying_type_t<EventCategory>>(lhs) ^
			static_cast<STL::underlying_type_t<EventCategory>>(rhs)
			);
	}

	EventCategory operator~(EventCategory category) {
		return static_cast<EventCategory>(
			~static_cast<STL::underlying_type_t<EventCategory>>(category)
			);
	}

	EventCategory& operator|=(EventCategory& lhs, EventCategory rhs) {
		lhs = static_cast<EventCategory>(
			static_cast<STL::underlying_type_t<EventCategory>>(lhs) |
			static_cast<STL::underlying_type_t<EventCategory>>(rhs)
			);
		return lhs;
	}

	EventCategory& operator&=(EventCategory& lhs, EventCategory rhs) {
		lhs = static_cast<EventCategory>(
			static_cast<STL::underlying_type_t<EventCategory>>(lhs) &
			static_cast<STL::underlying_type_t<EventCategory>>(rhs)
			);
		return lhs;
	}

	EventCategory& operator^=(EventCategory& lhs, EventCategory rhs) {
		lhs = static_cast<EventCategory>(
			static_cast<STL::underlying_type_t<EventCategory>>(lhs) ^
			static_cast<STL::underlying_type_t<EventCategory>>(rhs)
			);
		return lhs;
	}
}
