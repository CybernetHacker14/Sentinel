#pragma once

#include "Sentinel/Common/Core/Debug.h"
#include "Sentinel/Common/Core/Macros.h"
#include "Sentinel/Logging/Log.h"
#include "Sentinel/Filesystem/Filesystem.h"

#ifdef ST_ENABLE_ASSERTS

    // Alternatively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
    // provide support for custom formatting by concatenating the formatting string instead of having the format inside
    // the default message
    #define ST_INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
        {                                                  \
            if (!(check)) {                                \
                ST##type##ERROR(msg, __VA_ARGS__);         \
                ST_DEBUGBREAK();                           \
            }                                              \
        }
    #define ST_INTERNAL_ASSERT_WITH_MSG(type, check, ...) \
        ST_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
    #define ST_INTERNAL_ASSERT_NO_MSG(type, check)                            \
        ST_INTERNAL_ASSERT_IMPL(                                              \
            type,                                                             \
            check,                                                            \
            "Assertion '{0}' failed at {1}:{2}",                              \
            ST_STRINGIFY_MACRO(check),                                        \
            Sentinel::Filesystem::GetFilenameWithExtension(__LINE__).c_str(), \
            __LINE__)

    #define ST_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
    #define ST_INTERNAL_ASSERT_GET_MACRO(...) \
        ST_EXPAND_MACRO(ST_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, ST_INTERNAL_ASSERT_WITH_MSG, ST_INTERNAL_NO_MSG))

    // Currently accepts at least the condition and one additional parameter (the message) being optional
    #define ST_ASSERT(...)        ST_EXPAND_MACRO(ST_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
    #define ST_ENGINE_ASSERT(...) ST_EXPAND_MACRO(ST_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_ENGINE_, __VA_ARGS__))

#else

    #define ST_ASSERT(...)
    #define ST_ENGINE_ASSERT(...)

#endif  // ST_ENABLE_ASSERTS
