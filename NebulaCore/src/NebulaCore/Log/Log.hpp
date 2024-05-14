#pragma once

#include <spdlog/spdlog.h>

#include "../Util/Assert.hpp"

namespace Nebula
{
    class Log
    {
    public:
        static bool Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger()
        {
            NEBULA_INTERNAL_ASSERT(s_CoreLogger != nullptr, "Core logger is null!");
            return s_CoreLogger;
        }

        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
            NEBULA_INTERNAL_ASSERT(s_ClientLogger != nullptr, "Client logger is null!");
            return s_ClientLogger;
        }
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
} // namespace Nebula

// Core log macros

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEB_CORE_LOG_FATAL(...) SPDLOG_LOGGER_CRITICAL(::Nebula::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEB_CORE_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(::Nebula::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEB_CORE_LOG_WARN(...) SPDLOG_LOGGER_WARN(::Nebula::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEB_CORE_LOG_INFO(...) SPDLOG_LOGGER_INFO(::Nebula::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEB_CORE_LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(::Nebula::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEB_CORE_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(::Nebula::Log::GetCoreLogger(), __VA_ARGS__)

// Client log macros
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEB_LOG_FATAL(...) SPDLOG_LOGGER_CRITICAL(::Nebula::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEB_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(::Nebula::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEB_LOG_WARN(...) SPDLOG_LOGGER_WARN(::Nebula::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEB_LOG_INFO(...) SPDLOG_LOGGER_INFO(::Nebula::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEB_LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(::Nebula::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEB_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(::Nebula::Log::GetClientLogger(), __VA_ARGS__)
