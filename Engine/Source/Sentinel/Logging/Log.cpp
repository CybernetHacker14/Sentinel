#include "stpch.h"
#include "Sentinel/Logging/Log.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

namespace Sentinel {
    template<typename... Args>
    inline void Log::PrintTrace(CChar* format, Args&&... args) {
        printf_s(ANSI_COLOR_GREEN format ANSI_COLOR_RESET, args...);
    }

    template<typename... Args>
    inline void Log::PrintInfo(CChar* format, Args&&... args) {
    }

    template<typename... Args>
    inline void Log::PrintWarn(CChar* format, Args&&... args) {
    }

    template<typename... Args>
    inline void Log::PrintError(CChar* format, Args&&... args) {
    }

}  // namespace Sentinel
