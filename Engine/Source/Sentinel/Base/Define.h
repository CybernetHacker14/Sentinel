#pragma once

#include <memory>
#include "Sentinel/Base/PlatformDetection.h"
#include "Sentinel/Base/STLSwitch.h"
#include "Sentinel/Base/Ref.h"

#ifdef ST_DEBUG

#if defined(ST_PLATFORM_WINDOWS)
#define ST_DEBUGBREAK() __debugbreak()

#elif defined(ST_PLATFORM_LINUX)
#include <signal.h>
#define ST_DEBUGBREAK() raise(SIGTRAP)
#endif

#define ST_ENABLE_ASSERTS

#else
#define ST_DEBUGBREAK()
#endif // ST_DEBUG

#define ST_EXPAND_MACRO(x) x
#define ST_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define ST_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(Sentinel::STL::forward<decltype(args)>(args)...); }

namespace Sentinel
{
	template<typename T>
	using Scope = STL::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return STL::make_unique<T>(STL::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = IntrusiveRefHandle<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return IntrusiveRefHandle<T>::CreateRefHandle(args...);
	}
}

#include "Sentinel/Logging/Log.h"
#include "Sentinel/Base/Assert.h"
