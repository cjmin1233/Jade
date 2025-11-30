#pragma once
#include <memory>

#include "Jade/Core/PlatformDetection.h"

#ifdef JADE_DEBUG
    #define JADE_ENABLE_ASSERTS

    #if defined(JADE_PLATFORM_WINDOWS)
        #define JADE_DEBUGBREAK() __debugbreak()
    #elif defined(JADE_PLATFORM_LINUX)
        #include <signal.h>
        #define JADE_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
#else
    #define JADE_DEBUGBREAK()
#endif

#define JADE_EXPAND_MACRO(x) x
#define JADE_STRINGIFY_MACRO(x) #x

#define FMT_UNICODE 0   // Disable unicode support in fmt library

#define BIT(x) (1 << x)
//#define JADE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define JADE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Jade
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
