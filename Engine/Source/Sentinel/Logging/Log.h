#pragma once

#include "Sentinel/Common/Common.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Sentinel
{
	using TerminalLogger = spdlog::logger;

	// Logging class used for terminal
	class Log {
	public:
		static void Init();

		static std::shared_ptr<TerminalLogger>& GetEngineLogger() { return s_EngineLogger; }
		static std::shared_ptr<TerminalLogger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<TerminalLogger> s_EngineLogger;
		static std::shared_ptr<TerminalLogger> s_ClientLogger;
	};
}

// Engine log macros

#define ST_ENGINE_TRACE(...)     ::Sentinel::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define ST_ENGINE_INFO(...)      ::Sentinel::Log::GetEngineLogger()->info(__VA_ARGS__)
#define ST_ENGINE_WARN(...)      ::Sentinel::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define ST_ENGINE_ERROR(...)     ::Sentinel::Log::GetEngineLogger()->error(__VA_ARGS__)
#define ST_ENGINE_CRITICAL(...)  ::Sentinel::Log::GetEngineLogger()->critical(__VA_ARGS__)

// Client log macros

#define ST_TRACE(...)            ::Sentinel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ST_INFO(...)             ::Sentinel::Log::GetClientLogger()->info(__VA_ARGS__)
#define ST_WARN(...)             ::Sentinel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ST_ERROR(...)            ::Sentinel::Log::GetClientLogger()->error(__VA_ARGS__)
#define ST_CRITICAL(...)         ::Sentinel::Log::GetClientLogger()->critical(__VA_ARGS__)
