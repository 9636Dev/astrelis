#include "Log.hpp"
#define NEB_INCLUDE_LIBC_ASSERT
#include "Assert.hpp"

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Nebula
{
    void Log::Init()
    {
        static bool initialized = false;
        if (initialized)
        {
            return;
        }

        auto formatter = std::make_unique<spdlog::pattern_formatter>("%^[%T] %n: %v%$");
        s_CoreLogger   = spdlog::stdout_color_mt("NEBULA");
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->set_formatter(std::move(formatter));

        formatter      = std::make_unique<spdlog::pattern_formatter>("%^[%T] %n: %v%$");
        s_ClientLogger = spdlog::stdout_color_mt("APP");
        s_ClientLogger->set_level(spdlog::level::trace);
        s_ClientLogger->set_formatter(std::move(formatter));
    }

    std::shared_ptr<spdlog::logger>& Log::GetCoreLogger()
    {
        NEB_ASSERT(s_CoreLogger != nullptr, "Core logger not initialized!");
        return s_CoreLogger;
    }

    std::shared_ptr<spdlog::logger>& Log::GetClientLogger()
    {
        NEB_ASSERT(s_ClientLogger != nullptr, "Client logger not initialized!");
        return s_ClientLogger;
    }
} // namespace Nebula
