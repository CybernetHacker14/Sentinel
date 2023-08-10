#include "stpch.h"
#include "Sentinel/Events/Event.h"

#include "Sentinel/Common/Core/Macros.h"

#define MAX_LISTENERS_PER_TYPE 8

namespace Sentinel {
    namespace EventUtils {
        struct EventListener {
            EventsAPI::Callback Callback;
            void* Listener = nullptr;
        };

        // This structure isn't aligned
        struct EventListenerList {
            EventListener EventListeners[MAX_LISTENERS_PER_TYPE];
        };

        EventListenerList s_ListenerList[12];
    }  // namespace EventUtils

    Int16 EventsAPI::RegisterEvent(EventType type, void* listener, Callback callback) {
        for (UInt8 i = 0; i < MAX_LISTENERS_PER_TYPE; i++) {
            if (EventUtils::s_ListenerList[(UInt8)type].EventListeners[i].Listener == nullptr) {
                EventUtils::s_ListenerList[(UInt8)type].EventListeners[i].Listener = listener;
                EventUtils::s_ListenerList[(UInt8)type].EventListeners[i].Callback = ST_MOV(callback);
                return i;
            }
        }

        return -1;
    }

    void EventsAPI::UnregisterEvent(EventType type, Int16 entry) {
        EventUtils::s_ListenerList[(UInt8)type].EventListeners[entry].Listener = nullptr;
    }

    Bool EventsAPI::FireEvent(EventType type, EventData data) {
        for (UInt8 i = 0; i < MAX_LISTENERS_PER_TYPE; i++) {
            if (EventUtils::s_ListenerList[(UInt8)type].EventListeners[i].Listener == nullptr) continue;

            if (EventUtils::s_ListenerList[(UInt8)type].EventListeners[i].Callback(
                    type, data, EventUtils::s_ListenerList[(UInt8)type].EventListeners[i].Listener))
                return true;
        }

        return false;
    }
}  // namespace Sentinel
