#pragma once

#define ST_EXPAND_MACRO(x)    x
#define ST_STRINGIFY_MACRO(x) #x

#define ST_MAX(A, B) (((A) > (B)) ? (A) : (B))
#define ST_MIN(A, B) (((A) < (B)) ? (A) : (B))

#define BIT(x) (1 << x)

#define ST_BIND_FN(fn)                                                    \
    [this](auto&&... args) -> decltype(auto) {                            \
        return this->fn(Sentinel::STL::forward<decltype(args)>(args)...); \
    }
