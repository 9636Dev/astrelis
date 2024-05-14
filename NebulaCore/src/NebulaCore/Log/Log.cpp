#include "Log.hpp"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <iostream>

namespace Nebula
{
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    bool Log::Init()
    {
        try
        {
            s_CoreLogger = spdlog::stdout_color_mt("Nebula");
            s_CoreLogger->set_level(spdlog::level::trace);
            s_CoreLogger->set_pattern("%^[%T] %n: %v%$");

            s_ClientLogger = spdlog::stdout_color_mt("App");
            s_ClientLogger->set_level(spdlog::level::trace);
            s_ClientLogger->set_pattern("%^[%T] %n: %v%$");
            return true;
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            std::cerr << "Log initialization failed: " << ex.what() << std::endl;
            return false;
        }
    }
} // namespace Nebula
