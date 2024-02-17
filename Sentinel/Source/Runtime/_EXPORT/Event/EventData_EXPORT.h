#ifndef _EXPORT_EVENT_EVENTDATA_EXPORT_H
#define _EXPORT_EVENT_EVENTDATA_EXPORT_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

#include <stdint.h>

typedef enum EventType {
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    KeyReleased,
    KeyPressed,
    KeyRepeat,
    KeyTyped,
    MouseButtonReleased,
    MouseButtonPressed,
    MouseMoved,
    MouseScrolled,
    Event_MAX  // Do not use this. For internal type count purposes only.
} EventType;

// Taken from travisvroman/kohi
// A 128-bit wide data type, in which we can pass all sorts of data
typedef union EventData {
    long long Int64[2];
    unsigned long long UInt64[2];
    double Double[2];

    int Int32[4];
    unsigned int UInt32[4];
    float Float[4];

    short Int16[8];
    unsigned short UInt16[8];

    signed char Int8[16];
    unsigned char UInt8[16];

    char Char[16];
} EventData;

#endif  // !_EXPORT_EVENT_EVENTDATA_EXPORT_H
