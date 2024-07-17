#include "Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

#include <iostream>

namespace Nebula
{
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    bool Log::Init(Log::LogMode mode, spdlog::level::level_enum logLevel)
    {
        auto modeValue = static_cast<std::uint8_t>(mode);
        try
        {
            s_CoreLogger = spdlog::stdout_color_mt("NEBULA");
            s_CoreLogger->set_pattern("%^[%T] %n %s:%# | %v%$");

            if ((modeValue & 0x01) == 0)
            {
                s_CoreLogger->sinks().clear();
            }

            s_ClientLogger = spdlog::stdout_color_mt("APP");
            s_ClientLogger->set_pattern("%^[%T] %n %s:%# | %v%$");

            if ((modeValue & 0x02) == 0)
            {
                s_ClientLogger->sinks().clear();
            }

            s_CoreLogger->set_level(logLevel);
            s_ClientLogger->set_level(logLevel);

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

    void Log::AddClientSink(const std::shared_ptr<spdlog::sinks::sink>& sink)
    {
        s_ClientLogger->sinks().push_back(sink);
    }

    void Log::RemoveClientSink(const std::shared_ptr<spdlog::sinks::sink>& sink)
    {
        auto iter = std::find(s_ClientLogger->sinks().begin(), s_ClientLogger->sinks().end(), sink);
        if (iter != s_ClientLogger->sinks().end())
        {
            s_ClientLogger->sinks().erase(iter);
        }
    }

} // namespace Nebula
