#pragma once

#include "Core.hpp"

#include <memory>
#include <spdlog/spdlog.h>

namespace Nebula
{
    class NEBULA_CORE_API Log
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetCoreLogger();
        static std::shared_ptr<spdlog::logger>& GetClientLogger();

    private:
        inline static std::shared_ptr<spdlog::logger> s_CoreLogger; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
        inline static std::shared_ptr<spdlog::logger> s_ClientLogger; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace Nebula

// Core log macros
#define NEB_CORE_LOG_FATAL(...) SPDLOG_LOGGER_CRITICAL(::Nebula::Log::GetCoreLogger(), __VA_ARGS__) // NOLINT(cppcoreguidelines-macro-usage)
#define NEB_CORE_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(::Nebula::Log::GetCoreLogger(), __VA_ARGS__) // NOLINT(cppcoreguidelines-macro-usage)
#define NEB_CORE_LOG_WARN(...) SPDLOG_LOGGER_WARN(::Nebula::Log::GetCoreLogger(), __VA_ARGS__) // NOLINT(cppcoreguidelines-macro-usage)
#define NEB_CORE_LOG_INFO(...) SPDLOG_LOGGER_INFO(::Nebula::Log::GetCoreLogger(), __VA_ARGS__) // NOLINT(cppcoreguidelines-macro-usage)
#define NEB_CORE_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(::Nebula::Log::GetCoreLogger(), __VA_ARGS__) // NOLINT(cppcoreguidelines-macro-usage)

// Client log macros
#define NEB_LOG_FATAL(...) SPDLOG_LOGGER_CRITICAL(::Nebula::Log::GetClientLogger(), __VA_ARGS__) // NOLINT(cppcoreguidelines-macro-usage)
#define NEB_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(::Nebula::Log::GetClientLogger(), __VA_ARGS__) // NOLINT(cppcoreguidelines-macro-usage)
#define NEB_LOG_WARN(...) SPDLOG_LOGGER_WARN(::Nebula::Log::GetClientLogger(), __VA_ARGS__) // NOLINT(cppcoreguidelines-macro-usage)
#define NEB_LOG_INFO(...) SPDLOG_LOGGER_INFO(::Nebula::Log::GetClientLogger(), __VA_ARGS__) // NOLINT(cppcoreguidelines-macro-usage)
#define NEB_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(::Nebula::Log::GetClientLogger(), __VA_ARGS__) // NOLINT(cppcoreguidelines-macro-usage)
