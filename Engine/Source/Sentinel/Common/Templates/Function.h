#pragma once

#include <type_traits>

// https://gist.github.com/twoscomplement/030818a6c38c5a983482dc3a385a3ab8

namespace Sentinel {
    template<typename>
    struct Function;  // intentionally not defined

    template<typename R, typename... Args>
    struct Function<R(Args...)> {
        using Dispatcher = R (*)(void*, Args...);

        Dispatcher m_Dispatcher;  // A pointer to the static function that will call the
                                  // wrapped invokable object
        void* m_Target;           // A pointer to the invokable object

        // Dispatch() is instantiated by the Function constructor,
        // which will store a pointer to the function in m_Dispatcher.
        template<typename S>
        static R Dispatch(void* target, Args... args) {
            return (*(S*)target)(args...);
        }

        Function() = default;

        template<typename T>
        Function(T&& target) : m_Dispatcher(&Dispatch<typename std::decay<T>::type>), m_Target(&target) {}

        // Specialize for reference-to-function, to ensure that a valid pointer is
        // stored.
        using TargetFunctionRef = R(Args...);
        Function(TargetFunctionRef target) : m_Dispatcher(Dispatch<TargetFunctionRef>) {
            // It will not be possible to pass functions by reference on this platform.
            // Please use explicit function pointers i.e. foo(target) -> foo(&target)
            ST_STATIC_ASSERT(sizeof(void*) == sizeof target);
            m_Target = (void*)target;
        }

        R operator()(Args... args) const { return m_Dispatcher(m_Target, args...); }
    };
}  // namespace Sentinel