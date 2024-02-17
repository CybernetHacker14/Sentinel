#include "stpch.h"
#include "_EXPORT/Event/EventAPI_EXPORT.h"
#include "_EXPORT/Event/EventData_EXPORT.h"
#include "Common/Core/Macros.h"
#include "Event/Event.h"

#define MAX_LISTENERS_PER_TYPE 4

static inline Int GetFirstTrailingNonSetBit(UInt number) {
    static const UChar table[32] = {0,  1,  28, 2,  29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4,  8,
                                    31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6,  11, 5,  10, 9};

    return table[(((~number) & (0 - (~number))) * 0x077CB531) >> 27];
}

typedef struct EventListener {
    int (*callback)(EventType, EventData, void *);
    void *listener;
} EventListener;

typedef struct EventListenerList {
    EventListener listenerList[MAX_LISTENERS_PER_TYPE];
} EventListenerList;

static EventListenerList eventListeners[(Int)Event_MAX] = {0};

static UInt eventListenersAllocationMask[(Int)Event_MAX] = {0};

ST_API short Sentinel_EventAPI_RegisterEvent(
    EventType type, void *listener, int (*callback)(EventType, EventData, void *)) {
    Int next = GetFirstTrailingNonSetBit(eventListenersAllocationMask[(Int)type]);

    if (next >= MAX_LISTENERS_PER_TYPE) return -1;

    eventListeners[(Int)type].listenerList[next].listener = listener;
    eventListeners[(Int)type].listenerList[next].callback = callback;

    eventListenersAllocationMask[(Int)type] |= ST_BIT(next);

    return (short)next;
}

ST_API void Sentinel_EventAPI_UnregisterEvent(EventType type, short index) {
    eventListenersAllocationMask[(Int)type] &= ~ST_BIT((Int)index);
    eventListeners[(Int)type].listenerList[index].listener = NULL;
    eventListeners[(Int)type].listenerList[index].callback = NULL;
}

Int Sentinel_Event_FireEvent(EventType type, EventData data) {
    for (short i = 0; i < MAX_LISTENERS_PER_TYPE; i++) {
        if (eventListenersAllocationMask[(Int)type] & (ST_BIT(i))) {
            if (eventListeners[(Int)type].listenerList[i].callback(
                    type, data, eventListeners[(Int)type].listenerList[i].listener))
                return 1;
        }
    }

    return 0;
}
