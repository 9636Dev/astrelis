#include "TimerInstrumentor.hpp"

namespace Nebula::Profiling
{
    TimerInstrumentor::Scoped::Scoped()
        : m_StartTimepoint(std::chrono::high_resolution_clock::now())
    {
    }

    void TimerInstrumentor::Scoped::End(std::stringstream& sstream)
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        auto duration = end - start;
        sstream << "Duration: " << duration << "us\n";
    }

    std::unique_ptr<BaseInstrumentor::Scoped> TimerInstrumentor::Scope()
    {
        return std::make_unique<Scoped>();
    }
}  // namespace Nebula::Profiling
