#pragma once

#include <spdlog/spdlog.h>

namespace Astrelis {
    class Log {
    public:
        enum class LogMode : uint8_t {
            None        = 0,
            CoreOnly    = 1,
            ClientOnly  = 2,
            FullLogging = 3,
        };
        static bool Init(LogMode      mode = LogMode::FullLogging,
            spdlog::level::level_enum logLevel =
#ifdef ASTRELIS_DEBUG
                spdlog::level::trace
#else
                spdlog::level::info
#endif

        );

        static std::shared_ptr<spdlog::logger>& GetCoreLogger();
        static std::shared_ptr<spdlog::logger>& GetClientLogger();

        static bool IsInitialized() {
            return s_Initialized;
        }

        static void SetInitialized(bool initialized) {
            s_Initialized = initialized;
        }

        static void AddCoreSink(const std::shared_ptr<spdlog::sinks::sink>& sink);
        static void RemoveCoreSink(const std::shared_ptr<spdlog::sinks::sink>& sink);
        static void AddClientSink(const std::shared_ptr<spdlog::sinks::sink>& sink);
        static void RemoveClientSink(const std::shared_ptr<spdlog::sinks::sink>& sink);
    private:
        static bool                            s_Initialized;
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
} // namespace Astrelis

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASTRELIS_CORE_LOG_FATAL(...) \
    SPDLOG_LOGGER_CRITICAL(::Astrelis::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASTRELIS_CORE_LOG_ERROR(...) \
    SPDLOG_LOGGER_ERROR(::Astrelis::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASTRELIS_CORE_LOG_WARN(...) \
    SPDLOG_LOGGER_WARN(::Astrelis::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASTRELIS_CORE_LOG_INFO(...) \
    SPDLOG_LOGGER_INFO(::Astrelis::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASTRELIS_CORE_LOG_DEBUG(...) \
    SPDLOG_LOGGER_DEBUG(::Astrelis::Log::GetCoreLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASTRELIS_CORE_LOG_TRACE(...) \
    SPDLOG_LOGGER_TRACE(::Astrelis::Log::GetCoreLogger(), __VA_ARGS__)

// Client log macros
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASTRELIS_LOG_FATAL(...) \
    SPDLOG_LOGGER_CRITICAL(::Astrelis::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASTRELIS_LOG_ERROR(...) SPDLOG_LOGGER_ERROR(::Astrelis::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASTRELIS_LOG_WARN(...) SPDLOG_LOGGER_WARN(::Astrelis::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASTRELIS_LOG_INFO(...) SPDLOG_LOGGER_INFO(::Astrelis::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASTRELIS_LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(::Astrelis::Log::GetClientLogger(), __VA_ARGS__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define ASTRELIS_LOG_TRACE(...) SPDLOG_LOGGER_TRACE(::Astrelis::Log::GetClientLogger(), __VA_ARGS__)
