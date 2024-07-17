#pragma once

#include <spdlog/spdlog.h>

namespace Nebula
{
    class Log
    {
    public:
        enum class LogMode : uint8_t
        {
            None        = 0,
            CoreOnly    = 1,
            ClientOnly  = 2,
            FullLogging = 3,
        };
        static bool Init(LogMode mode = LogMode::FullLogging,
                         spdlog::level::level_enum logLevel =
#ifdef NEBULA_DEBUG
                             spdlog::level::trace
#else
                             spdlog::level::info
#endif

        );

        static std::shared_ptr<spdlog::logger>& GetCoreLogger();
        static std::shared_ptr<spdlog::logger>& GetClientLogger();

        static bool IsInitialized() { return s_Initialized; }

        static void SetInitialized(bool initialized) { s_Initialized = initialized; }

        static void AddCoreSink(const std::shared_ptr<spdlog::sinks::sink>& sink);
        static void RemoveCoreSink(const std::shared_ptr<spdlog::sinks::sink>& sink);
        static void AddClientSink(const std::shared_ptr<spdlog::sinks::sink>& sink);
        static void RemoveClientSink(const std::shared_ptr<spdlog::sinks::sink>& sink);
    private:
        static bool s_Initialized;
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
} // namespace Nebula

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEBULA_CORE_LOG_FATAL(...) SPDLOG_LOGGER_CRITICAL(::Nebula::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEBULA_CORE_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(::Nebula::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEBULA_CORE_LOG_WARN(...) SPDLOG_LOGGER_WARN(::Nebula::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEBULA_CORE_LOG_INFO(...) SPDLOG_LOGGER_INFO(::Nebula::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEBULA_CORE_LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(::Nebula::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEBULA_CORE_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(::Nebula::Log::GetCoreLogger(), __VA_ARGS__)

// Client log macros
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEBULA_LOG_FATAL(...) SPDLOG_LOGGER_CRITICAL(::Nebula::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEBULA_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(::Nebula::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEBULA_LOG_WARN(...) SPDLOG_LOGGER_WARN(::Nebula::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEBULA_LOG_INFO(...) SPDLOG_LOGGER_INFO(::Nebula::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEBULA_LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(::Nebula::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define NEBULA_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(::Nebula::Log::GetClientLogger(), __VA_ARGS__)
