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

#define FMT_UNICODE 0   // Disable unicode support in fmt library

#define BIT(x) (1 << x)
#define PURE = 0