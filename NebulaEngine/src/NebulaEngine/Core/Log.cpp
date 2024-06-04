#include "Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

#include <iostream>

namespace Nebula
{
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    bool Log::Init()
    {
        try
        {
            s_CoreLogger = spdlog::stdout_color_mt("NEBULA");
            s_CoreLogger->set_pattern("%^[%T] %n %s:%# | %v%$");

            s_ClientLogger = spdlog::stdout_color_mt("APP");
            s_ClientLogger->set_pattern("%^[%T] %n %s:%# | %v%$");

#ifdef NEBULA_DEBUG
            s_CoreLogger->set_level(spdlog::level::trace);
            s_ClientLogger->set_level(spdlog::level::trace);
#endif

            return true;
        }
        catch (const spdlog::spdlog_ex& e)
        {
            std::cerr << "Log initialization failed: " << e.what() << std::endl;
            return false;
        }
    }

    std::shared_ptr<spdlog::logger>& Log::GetCoreLogger() { return s_CoreLogger; }

    std::shared_ptr<spdlog::logger>& Log::GetClientLogger() { return s_ClientLogger; }
} // namespace Nebula
