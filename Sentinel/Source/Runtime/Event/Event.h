#ifndef EVENT_EVENT_H
#define EVENT_EVENT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include "Common/Core/DataTypes.h"

typedef enum EventType EventType;
typedef union EventData EventData;

Int Sentinel_Event_FireEvent(EventType type, EventData data);

#endif  // !EVENT_EVENT_H
