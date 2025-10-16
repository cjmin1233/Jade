#pragma once
#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Jade
{
    class JADE_API Log
    {
    public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

// Core log macros
#define JADE_CORE_TRACE(...)            ::Jade::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define JADE_CORE_DEBUG(...)            ::Jade::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define JADE_CORE_INFO(...)             ::Jade::Log::GetCoreLogger()->info(__VA_ARGS__)
#define JADE_CORE_WARN(...)             ::Jade::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define JADE_CORE_ERROR(...)            ::Jade::Log::GetCoreLogger()->error(__VA_ARGS__)
#define JADE_CORE_CRITICAL(...)         ::Jade::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define JADE_TRACE(...)                 ::Jade::Log::GetClientLogger()->trace(__VA_ARGS__)
#define JADE_DEBUG(...)                 ::Jade::Log::GetClientLogger()->debug(__VA_ARGS__)
#define JADE_INFO(...)                  ::Jade::Log::GetClientLogger()->info(__VA_ARGS__)
#define JADE_WARN(...)                  ::Jade::Log::GetClientLogger()->warn(__VA_ARGS__)
#define JADE_ERROR(...)                 ::Jade::Log::GetClientLogger()->error(__VA_ARGS__)
#define JADE_CRITICAL(...)              ::Jade::Log::GetClientLogger()->critical(__VA_ARGS__)