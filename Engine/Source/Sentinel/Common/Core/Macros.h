#pragma once

#define ST_EXPAND_MACRO(x)    x
#define ST_STRINGIFY_MACRO(x) #x

#define ST_MAX(A, B) (((A) > (B)) ? (A) : (B))
#define ST_MIN(A, B) (((A) < (B)) ? (A) : (B))

#define ST_BIT(x) (1 << x)

// https://www.foonathan.net/2020/09/move-forward/
#define ST_MOV(...) static_cast<std::remove_reference_t<decltype(__VA_ARGS__)> &&>(__VA_ARGS__)
#define ST_FWD(...) static_cast<decltype(__VA_ARGS__) &&>(__VA_ARGS__)

#define ST_BIND_FN(fn)                         \
    [this](auto &&...args) -> decltype(auto) { \
        return this->fn(ST_FWD(args)...);      \
    }

template<typename T, size_t N>
char (&SizeHelper(T (&x)[N]))[N];

template<typename T, size_t N>
char (&SizeHelper(T (&&x)[N]))[N];

#define ST_ARRAY_COUNT(x) (sizeof(SizeHelper(x)))
