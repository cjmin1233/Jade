#pragma once

#include "Jade/Core/Base.h"
#include "Jade/Core/Log.h"

#include <filesystem>

#ifdef JADE_ENABLE_ASSERTS

// Helper macros for assertions
#define JADE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { JADE##type##ERROR(msg, __VA_ARGS__); JADE_DEBUGBREAK();}}
#define JADE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) JADE_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define JADE_INTERNAL_ASSERT_NO_MSG(type, check) JADE_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", JADE_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

// Helper macro to select the correct assert macro overload
#define JADE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, name, ...) name
#define JADE_INTERNAL_ASSERT_GET_MACRO(...) JADE_EXPAND_MACRO( JADE_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, JADE_INTERNAL_ASSERT_WITH_MSG, JADE_INTERNAL_ASSERT_NO_MSG) )

// Main assert macros
#define JADE_ASSERT(...) JADE_EXPAND_MACRO(JADE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#define JADE_CORE_ASSERT(...) JADE_EXPAND_MACRO(JADE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))

#else
    #define JADE_ASSERT(x, ...)
    #define JADE_CORE_ASSERT(x, ...)
#endif // JADE_ENABLE_ASSERTS
