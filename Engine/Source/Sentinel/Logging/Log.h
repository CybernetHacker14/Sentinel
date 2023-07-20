#pragma once

#include "Sentinel/Common/Core/DataTypes.h"
#include "Sentinel/Common/Core/Debug.h"

namespace Sentinel {
    // Logging class used for terminal
    class Log {
    public:
        template<typename... Args>
        static void PrintTrace(CChar* format, Args&&... args);

        template<typename... Args>
        static void PrintInfo(CChar* format, Args&&... args);

        template<typename... Args>
        static void PrintWarn(CChar* format, Args&&... args);

        template<typename... Args>
        static void PrintError(CChar* format, Args&&... args);

    private:
        inline Log() = default;
    };
}  // namespace Sentinel

#ifdef ST_DEBUG
    #define ST_TERMINAL_TRACE(format, ...) ::Sentinel::Log::PrintTrace(format, __VA_ARGS__)
#else
    #define ST_TERMINAL_TRACE(format, ...)
#endif  // ST_DEBUG
