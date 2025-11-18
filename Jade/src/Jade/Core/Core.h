#pragma once
#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
    // Windows x64/x86
    #ifdef _WIN64
        // Windows x64
        #define JADE_PLATFORM_WINDOWS
    #else
        // Windows x86
        #error "x86 Builds are not supported!"
    #endif // _WIN64

#elif defined(__ANDROID__)
    #define JADE_PLATFORM_ANDROID
    #error "Android platform is not supported yet!"
#elif defined(__linux__)
    #define JADE_PLATFORM_LINUX
    #error "Linux platform is not supported yet!"
#else
    #error "Only Windows platform is supported!"
#endif // _WIN32

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

#ifdef JADE_ENABLE_ASSERTS
    #define JADE_ASSERT(x, ...) { if(!(x)) { JADE_ERROR("Assertion Failed: {0}", __VA_ARGS__); JADE_DEBUGBREAK(); } }
    #define JADE_CORE_ASSERT(x, ...) { if(!(x)) { JADE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); JADE_DEBUGBREAK(); } }
#else
    #define JADE_ASSERT(x, ...)
    #define JADE_CORE_ASSERT(x, ...)
#endif
                               
#define FMT_UNICODE 0   // Disable unicode support in fmt library

#define BIT(x) (1 << x)
#define JADE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

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