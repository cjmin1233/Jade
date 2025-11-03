#pragma once

#ifdef JADE_PLATFORM_WINDOWS
#if JADE_DYNAMIC_LINK
    #ifdef JADE_BUILD_DLL
        #define JADE_API __declspec(dllexport)
    #else
        #define JADE_API __declspec(dllimport)
    #endif  // JADE_BUILD_DLL
#else  
    #define JADE_API
#endif  // JADE_DYNAMIC_LINK
#else
    #error Jade only supports Windows for now!
#endif  // JADE_PLATFORM_WINDOWS

#ifdef JADE_DEBUG
#define JADE_ENABLE_ASSERTS
#endif

#ifdef JADE_ENABLE_ASSERTS
    #define JADE_ASSERT(x, ...) { if(!(x)) { JADE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define JADE_CORE_ASSERT(x, ...) { if(!(x)) { JADE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } } 
#else
    #define JADE_ASSERT(x, ...)
    #define JADE_CORE_ASSERT(x, ...)
#endif
                               
#define FMT_UNICODE 0   // Disable unicode support in fmt library

#define BIT(x) (1 << x)
#define JADE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#include <memory>

namespace Jade
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;
}