#ifndef _EXPORT_EVENT_EVENTAPI_EXPORT_H
#define _EXPORT_EVENT_EVENTAPI_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "../ExportConditional.h"

typedef enum EventType EventType;
typedef union EventData EventData;

// This returns an index, which can be passed to Unregister call, to remove the event
ST_API short Sentinel_EventAPI_RegisterEvent(EventType type, void* listener, int (*callback)(EventType, EventData, void*));

ST_API void Sentinel_EventAPI_UnregisterEvent(EventType type, short index);

#endif  // !_EXPORT_EVENT_EVENTAPI_EXPORT_H
