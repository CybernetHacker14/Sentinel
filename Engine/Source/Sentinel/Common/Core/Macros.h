#pragma once

#include "Sentinel/Common/Core/STL.h"

#define ST_EXPAND_MACRO(x) x
#define ST_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define ST_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(Sentinel::STL::forward<decltype(args)>(args)...); }
