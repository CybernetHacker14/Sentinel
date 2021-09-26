#include "stpch.h"
#include "Sentinel/Events/Event.h"

#include "Sentinel/Events/Categories/ApplicationEvent.h"
#include "Sentinel/Events/Categories/KeyEvent.h"
#include "Sentinel/Events/Categories/MouseEvent.h"
#include "Sentinel/Events/Categories/WindowEvent.h"

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

	EventCategory Event::GetEventCategoryFlags() {
		switch (Type)
		{
			case Sentinel::EventType::WindowClose:
				return BaseDowncast<WindowCloseEvent>()->GetEventCategoryFlags();
			case Sentinel::EventType::WindowResize:
				return BaseDowncast<WindowResizeEvent>()->GetEventCategoryFlags();
			case Sentinel::EventType::AppTick:
				return BaseDowncast<AppTickEvent>()->GetEventCategoryFlags();
			case Sentinel::EventType::AppUpdate:
				return BaseDowncast<AppUpdateEvent>()->GetEventCategoryFlags();
			case Sentinel::EventType::AppRender:
				return BaseDowncast<AppRenderEvent>()->GetEventCategoryFlags();
			case Sentinel::EventType::KeyPressed:
				return BaseDowncast<KeyPressedEvent>()->GetEventCategoryFlags();
			case Sentinel::EventType::KeyReleased:
				return BaseDowncast<KeyReleasedEvent>()->GetEventCategoryFlags();
			case Sentinel::EventType::KeyTyped:
				return BaseDowncast<KeyTypedEvent>()->GetEventCategoryFlags();
			case Sentinel::EventType::MouseButtonPressed:
				return BaseDowncast<MouseButtonPressedEvent>()->GetEventCategoryFlags();
			case Sentinel::EventType::MouseButtonReleased:
				return BaseDowncast<MouseButtonReleasedEvent>()->GetEventCategoryFlags();
			case Sentinel::EventType::MouseMoved:
				return BaseDowncast<MouseMovedEvent>()->GetEventCategoryFlags();
			case Sentinel::EventType::MouseScrolled:
				return BaseDowncast<MouseScrolledEvent>()->GetEventCategoryFlags();
		}

		ST_ENGINE_ASSERT(false, "Unknown Event fired");
		return EventCategory::None;
	}

	const char* Event::GetName() {
		switch (Type)
		{
			case Sentinel::EventType::WindowClose:
				return BaseDowncast<WindowCloseEvent>()->GetName();
			case Sentinel::EventType::WindowResize:
				return BaseDowncast<WindowResizeEvent>()->GetName();
			case Sentinel::EventType::AppTick:
				return BaseDowncast<AppTickEvent>()->GetName();
			case Sentinel::EventType::AppUpdate:
				return BaseDowncast<AppUpdateEvent>()->GetName();
			case Sentinel::EventType::AppRender:
				return BaseDowncast<AppRenderEvent>()->GetName();
			case Sentinel::EventType::KeyPressed:
				return BaseDowncast<KeyPressedEvent>()->GetName();
			case Sentinel::EventType::KeyReleased:
				return BaseDowncast<KeyReleasedEvent>()->GetName();
			case Sentinel::EventType::KeyTyped:
				return BaseDowncast<KeyTypedEvent>()->GetName();
			case Sentinel::EventType::MouseButtonPressed:
				return BaseDowncast<MouseButtonPressedEvent>()->GetName();
			case Sentinel::EventType::MouseButtonReleased:
				return BaseDowncast<MouseButtonReleasedEvent>()->GetName();
			case Sentinel::EventType::MouseMoved:
				return BaseDowncast<MouseMovedEvent>()->GetName();
			case Sentinel::EventType::MouseScrolled:
				return BaseDowncast<MouseScrolledEvent>()->GetName();
		}

		ST_ENGINE_ASSERT(false, "Unknown Event fired");
		return nullptr;
	}

	const STL::string& Event::ToString() {
		switch (Type)
		{
			case Sentinel::EventType::WindowClose:
				return BaseDowncast<WindowCloseEvent>()->ToString();
			case Sentinel::EventType::WindowResize:
				return BaseDowncast<WindowResizeEvent>()->ToString();
			case Sentinel::EventType::AppTick:
				return BaseDowncast<AppTickEvent>()->ToString();
			case Sentinel::EventType::AppUpdate:
				return BaseDowncast<AppUpdateEvent>()->ToString();
			case Sentinel::EventType::AppRender:
				return BaseDowncast<AppRenderEvent>()->ToString();
			case Sentinel::EventType::KeyPressed:
				return BaseDowncast<KeyPressedEvent>()->ToString();
			case Sentinel::EventType::KeyReleased:
				return BaseDowncast<KeyReleasedEvent>()->ToString();
			case Sentinel::EventType::KeyTyped:
				return BaseDowncast<KeyTypedEvent>()->ToString();
			case Sentinel::EventType::MouseButtonPressed:
				return BaseDowncast<MouseButtonPressedEvent>()->ToString();
			case Sentinel::EventType::MouseButtonReleased:
				return BaseDowncast<MouseButtonReleasedEvent>()->ToString();
			case Sentinel::EventType::MouseMoved:
				return BaseDowncast<MouseMovedEvent>()->ToString();
			case Sentinel::EventType::MouseScrolled:
				return BaseDowncast<MouseScrolledEvent>()->ToString();
		}

		ST_ENGINE_ASSERT(false, "Unknown Event fired");
		return STL::string();
	}
}
