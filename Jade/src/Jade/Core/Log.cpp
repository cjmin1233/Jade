#include "jdpch.h"

#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Jade
{
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init()
    {
        std::string pattern_string = "%^[%T] %n: %v%$";
        spdlog::set_pattern(pattern_string);

        s_CoreLogger = spdlog::stdout_color_mt("JADE");
        s_CoreLogger->set_level(spdlog::level::trace);

        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
    }
} // namespace Jade