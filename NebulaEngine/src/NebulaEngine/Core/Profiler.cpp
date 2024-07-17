#include "Profiler.hpp"

#include "Log.hpp"

#include "QProfile/Log.hpp"

namespace Nebula
{
    static void ProfilerLogFunction(QProfile::LogLevel level, const std::string& message)
    {
        if (Log::IsInitialized())
        {
            switch (level)
            {
            case QProfile::LogLevel::Info:
                NEBULA_CORE_LOG_INFO("[PROFILER]: {0}", message);
                break;
            case QProfile::LogLevel::Warning:
                NEBULA_CORE_LOG_WARN("[PROFILER]: {0}", message);
                break;
            case QProfile::LogLevel::Error:
                NEBULA_CORE_LOG_ERROR("[PROFILER]: {0}", message);
                break;
            default:
                break;
            };
            return;
        }

        switch (level)
        {
        case QProfile::LogLevel::Info:
            std::cout << "[PROFILER][INFO]: " << message << std::endl;
            break;
        case QProfile::LogLevel::Warning:
            std::cout << "[PROFILER][WARNING]: " << message << std::endl;
            break;
        case QProfile::LogLevel::Error:
            std::cout << "[PROFILER][ERROR]: " << message << std::endl;
            break;
        }
    }

    bool Profiling::Init()
    {
        QProfile::Log::s_LogFunction = ProfilerLogFunction;
        return true;
    }
} // namespace Nebula
