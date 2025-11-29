#pragma once
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
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    // Apple platforms
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "iOS Simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #define JADE_PLATFORM_IOS
        #error "iOS platform is not supported yet!"
    #elif TARGET_OS_MAC == 1
        #define JADE_PLATFORM_MACOS
        #error "MacOS platform is not supported yet!"
    #else
        #error "Unknown Apple platform!"
    #endif
#elif defined(__ANDROID__)
    #define JADE_PLATFORM_ANDROID
    #error "Android platform is not supported yet!"
#elif defined(__linux__)
    #define JADE_PLATFORM_LINUX
    #error "Linux platform is not supported yet!"
#else
    #error "Unknown platform!"
#endif // _Platform detection
