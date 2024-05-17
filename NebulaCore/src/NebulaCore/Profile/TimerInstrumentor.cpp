#include "TimerInstrumentor.hpp"

namespace Nebula::Profiling
{
    TimerInstrumentor::Scoped::Scoped() :
        m_StartTimepoint(std::chrono::high_resolution_clock::now()),
        m_EndTimepoint(m_StartTimepoint)
    {
    }

    void TimerInstrumentor::Scoped::End(JsonObject& json)
    {
        if (m_EndTimepoint == m_StartTimepoint)
        {
            ActualEnd();
        }
        std::chrono::duration<double, std::micro> duration = m_EndTimepoint - m_StartTimepoint;

        JsonObject timer;
        timer["duration"] = JsonValue(std::to_string(duration.count()));
        json.AddValue("timer", timer);
    }

    std::unique_ptr<BaseInstrumentor::Scoped> TimerInstrumentor::Scope() { return std::make_unique<Scoped>(); }
} // namespace Nebula::Profiling
