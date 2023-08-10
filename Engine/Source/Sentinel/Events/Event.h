#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Common/Templates/Function.h"

namespace Sentinel {
    // Enum for depicting the type of event
    enum class EventType : UInt8 {
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    // Taken from travisvroman/kohi
    // A 128-bit wide data type, in which we can pass all sorts of data
    union EventData {
        Int64 Int64[2];
        UInt64 UInt64[2];
        Double Double[2];

        Int32 Int32[4];
        UInt32 UInt32[4];
        Float Float[4];

        Int16 Int16[8];
        UInt16 UInt16[8];

        Int8 Int8[16];
        UInt8 UInt8[16];

        Char Char[16];
    };

    class EventsAPI {
    public:
        using Callback = Function<Bool(EventType, EventData, void*)>;

    public:
        static Int16 RegisterEvent(EventType type, void* listener, Callback callback);
        static void UnregisterEvent(EventType type, Int16 entry);
        static Bool FireEvent(EventType type, EventData data);
    };
}  // namespace Sentinel
