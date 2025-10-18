#pragma once

#ifdef JADE_PLATFORM_WINDOWS
    #ifdef JADE_BUILD_DLL
        #define JADE_API __declspec(dllexport)
    #else
        #define JADE_API __declspec(dllimport)
    #endif  // JADE_BUILD_DLL
#else
    #error Jade only supports Windows for now!
#endif  // JADE_PLATFORM_WINDOWS

#ifdef JADE_ENABLE_ASSERTS
    #define JADE_ASSERT(x, ...) { if(!(x)) { JADE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define JADE_CORE_ASSERT(x, ...) { if(!(x)) { JADE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } } 
#else
    #define JADE_ASSERT(x, ...)
    #define JADE_CORE_ASSERT(x, ...)
#endif
                               
#define FMT_UNICODE 0   // Disable unicode support in fmt library

#define BIT(x) (1 << x)
#define PURE = 0