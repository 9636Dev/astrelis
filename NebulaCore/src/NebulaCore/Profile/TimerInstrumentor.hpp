#pragma once

#include "BaseInstrumentor.hpp"

#include <chrono>
#include <memory>

namespace Nebula::Profiling
{
    class TimerInstrumentor : public BaseInstrumentor
    {
    public:
        class Scoped : public BaseInstrumentor::Scoped
        {
        public:
            Scoped();

            void End(JsonObject& json) override;

            void ActualEnd() override { m_EndTimepoint = std::chrono::high_resolution_clock::now(); }
        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_EndTimepoint;
        };

        TimerInstrumentor()                                    = default;
        TimerInstrumentor(const TimerInstrumentor&)            = delete;
        TimerInstrumentor(TimerInstrumentor&&)                 = delete;
        TimerInstrumentor& operator=(const TimerInstrumentor&) = delete;
        TimerInstrumentor& operator=(TimerInstrumentor&&)      = delete;
        ~TimerInstrumentor() override                          = default;

        std::unique_ptr<BaseInstrumentor::Scoped> Scope() override;
    };
} // namespace Nebula::Profiling


