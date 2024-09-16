#pragma once

#include <chrono>

namespace Astrelis {
    // TODO: Test, refactor, and document this class

    template<typename T = double, typename Ratio = std::ratio<1>> class TimeSpan {
    public:
        template<typename U, typename R> friend class TimeSpan;

        TimeSpan() noexcept : m_Duration(0) {
        }

        TimeSpan(const TimeSpan& other)            = default;
        TimeSpan(TimeSpan&& other)                 = default;
        TimeSpan& operator=(const TimeSpan& other) = default;
        TimeSpan& operator=(TimeSpan&& other)      = default;
        ~TimeSpan()                                = default;

        explicit TimeSpan(const std::chrono::duration<T, Ratio>& duration) : m_Duration(duration) {
        }

        template<typename T2, typename Ratio2>
        explicit TimeSpan(const TimeSpan<T2, Ratio2>& other)
            : m_Duration(
                  std::chrono::duration_cast<std::chrono::duration<T, Ratio>>(other.m_Duration)) {
        }

        template<typename T2, typename Ratio2>
        TimeSpan& operator=(const TimeSpan<T2, Ratio2>& other) {
            m_Duration =
                std::chrono::duration_cast<std::chrono::duration<T, Ratio>>(other.GetDuration());
            return *this;
        }

        template<typename T2, typename Ratio2> TimeSpan& operator=(TimeSpan<T2, Ratio2>&& other) {
            m_Duration =
                std::chrono::duration_cast<std::chrono::duration<T, Ratio>>(other.GetDuration());
            return *this;
        }

        template<typename T2, typename Ratio2>
        TimeSpan operator+(const TimeSpan<T2, Ratio2>& other) const {
            return TimeSpan(m_Duration
                + std::chrono::duration_cast<std::chrono::duration<T, Ratio>>(other.GetDuration()));
        }

        template<typename T2, typename Ratio2>
        TimeSpan operator-(const TimeSpan<T2, Ratio2>& other) const {
            return TimeSpan(m_Duration
                - std::chrono::duration_cast<std::chrono::duration<T, Ratio>>(other.m_Duration));
        }

        template<typename T2> TimeSpan operator*(T2 scalar) const {
            return TimeSpan(m_Duration * scalar);
        }

        template<typename T2> TimeSpan operator/(T2 scalar) const {
            return TimeSpan(m_Duration / scalar);
        }

        template<typename T2, typename Ratio2>
        bool operator==(const TimeSpan<T2, Ratio2>& other) const {
            return m_Duration
                == std::chrono::duration_cast<std::chrono::duration<T, Ratio>>(other.GetDuration());
        }

        template<typename T2, typename Ratio2>
        bool operator!=(const TimeSpan<T2, Ratio2>& other) const {
            return m_Duration
                != std::chrono::duration_cast<std::chrono::duration<T, Ratio>>(other.GetDuration());
        }

        // NOLINTNEXTLINE(hicpp-explicit-conversions, google-explicit-constructor)
        operator T() const {
            return m_Duration.count();
        }
    private:
        std::chrono::duration<T, Ratio> m_Duration;
    };

    using Milliseconds = TimeSpan<double, std::milli>;
    using Seconds      = TimeSpan<double>;

    class TimePoint {
    public:
        TimePoint() : m_TimePoint(std::chrono::high_resolution_clock::now()) {
        }

        TimePoint(const TimePoint& other)            = default;
        TimePoint(TimePoint&& other)                 = default;
        TimePoint& operator=(const TimePoint& other) = default;
        TimePoint& operator=(TimePoint&& other)      = default;
        ~TimePoint()                                 = default;

        static TimePoint Now() {
            return TimePoint();
        }
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_TimePoint;

        friend class Time;
    };

    class Time {
    public:
        friend class Application;

        static Seconds DeltaTime() {
            return Seconds(s_DeltaTime);
        }

        static Milliseconds DeltaTimeMillis() {
            return s_DeltaTime;
        }

        static TimePoint Now() {
            return TimePoint::Now();
        }

        template<typename T>
            requires std::is_same_v<T, Milliseconds> || std::is_same_v<T, Seconds>
        static T ElapsedTime(const TimePoint& start, const TimePoint& end) {
            return T(end.m_TimePoint - start.m_TimePoint);
        }
    private:
        static Milliseconds s_DeltaTime;
    };
} // namespace Astrelis
