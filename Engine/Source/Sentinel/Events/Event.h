#pragma once

#include "Sentinel/Common/Common.h"

#include <sstream>

namespace Sentinel {
    // Enum for depicting the type of event
    enum class EventType : UInt8 {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        AppTick,
        AppUpdate,
        AppRender,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    // Enum for depicting the category of event
    enum class EventCategory : UInt8 {
        None = 0,
        Application = BIT(0),
        Input = BIT(1),
        Keyboard = BIT(2),
        Mouse = BIT(3),
        MouseButton = BIT(4)
    };

    // Bitwise operations for enum EventCategory

    EventCategory operator|(EventCategory lhs, EventCategory rhs);
    EventCategory operator&(EventCategory lhs, EventCategory rhs);
    EventCategory operator^(EventCategory lhs, EventCategory rhs);
    EventCategory operator~(EventCategory category);
    EventCategory& operator|=(EventCategory& lhs, EventCategory rhs);
    EventCategory& operator&=(EventCategory& lhs, EventCategory rhs);
    EventCategory& operator^=(EventCategory& lhs, EventCategory rhs);

    class Event {
    public:
        inline const EventCategory GetEventCategoryFlags() const {
            if (!m_GetFlagsFunction) return EventCategory();

            return m_GetFlagsFunction();
        }

        inline const char* GetName() const {
            if (!m_GetNameFunction) return "";

            return m_GetNameFunction();
        }

        inline const STL::string& ToString() const {
            if (!m_ToStringFunction) return STL::string();

            return m_ToStringFunction();
        }

        inline const EventType GetEventType() const { return Type; }

        inline Bool IsInCategory(EventCategory category) const {
            return static_cast<Bool>(GetEventCategoryFlags() & category);
        }

    public:
        template<typename T>
        inline T* Cast() {
            static_assert(STL::is_base_of<Event, T>::value, "'T' should be a derived from Event.");
            return static_cast<T*>(this);
        }

    public:
        Bool Handled = false;

    protected:
        EventType Type = EventType::None;

    protected:
        STL::delegate<const EventCategory()> m_GetFlagsFunction;
        STL::delegate<const char*()> m_GetNameFunction;
        STL::delegate<const STL::string&()> m_ToStringFunction;
    };
}  // namespace Sentinel
