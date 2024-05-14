#pragma once

#include "BaseInstrumentor.hpp"
#include "TimerInstrumentor.hpp"

#include <memory>
#include <string>
#include <vector>

// TODO(9636Dev): Implement MemoryInstrumentor, make sure it is thread-safe

namespace Nebula::Profiling
{
    class Instrumentor
    {
        friend class TimerInstrumentor;
        friend class MemoryInstrumentor;
    public:
        class Scoped
        {
        public:
            explicit Scoped(Instrumentor& instrumentor, const std::string& name);
            ~Scoped();
            Scoped(const Scoped&)            = delete;
            Scoped(Scoped&&)                 = delete;
            Scoped& operator=(const Scoped&) = delete;
            Scoped& operator=(Scoped&&)      = delete;
        private:
            Instrumentor& m_Instrumentor;
            std::string m_Name;
            std::vector<std::unique_ptr<BaseInstrumentor::Scoped>> m_Scopes;
        };

        inline void AddInstrumentor(std::unique_ptr<BaseInstrumentor>&& instrumentor) { m_Instrumentors.push_back(std::move(instrumentor)); }

        void BeginSession(const std::string& name, const std::string& filepath = "results.json");
        void EndSession();

        Scoped Scope(const std::string& name);

        inline static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }

        Instrumentor(const Instrumentor&)            = delete;
        Instrumentor(Instrumentor&&)                 = delete;
        Instrumentor& operator=(const Instrumentor&) = delete;
        Instrumentor& operator=(Instrumentor&&)      = delete;
    private:
        Instrumentor();
        ~Instrumentor();

        std::string m_SessionName;
        std::string m_Filepath;
        std::vector<std::unique_ptr<BaseInstrumentor>> m_Instrumentors;
        std::stringstream m_Sstream;
    };
} // namespace Nebula::Profiling
