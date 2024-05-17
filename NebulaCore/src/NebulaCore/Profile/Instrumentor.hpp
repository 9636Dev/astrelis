#pragma once

#include "BaseInstrumentor.hpp"
#include "BasicJson.hpp"
#include "TimerInstrumentor.hpp"

#include <memory>
#include <source_location>
#include <string>
#include <unordered_map>
#include <vector>

namespace Nebula::Profiling
{
    class Instrumentor
    {
        friend class TimerInstrumentor;
        //friend class MemoryInstrumentor;
    public:
        class Scoped
        {
        public:
            explicit Scoped(Instrumentor& instrumentor, const std::string& name);

            virtual ~Scoped() { End(); }

            Scoped(const Scoped&)            = delete;
            Scoped(Scoped&&)                 = delete;
            Scoped& operator=(const Scoped&) = delete;
            Scoped& operator=(Scoped&&)      = delete;

            virtual void End();
        protected:
            bool m_Ended = false;         // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
            Instrumentor& m_Instrumentor; // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
            std::string m_Name;           // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
            std::vector<std::unique_ptr<BaseInstrumentor::Scoped>>
                m_Scopes;                 // NOLINT(cppcoreguidelines-non-private-member-variables-in-classes)
        };

        class FunctionScoped : public Scoped
        {
        public:
            explicit FunctionScoped(Instrumentor& instrumentor, const std::source_location& function);

            ~FunctionScoped() override { PreDestroy(); }

            FunctionScoped(const FunctionScoped&)            = delete;
            FunctionScoped(FunctionScoped&&)                 = delete;
            FunctionScoped& operator=(const FunctionScoped&) = delete;
            FunctionScoped& operator=(FunctionScoped&&)      = delete;

            void PreDestroy();
        };

        inline void AddInstrumentor(std::unique_ptr<BaseInstrumentor>&& instrumentor)
        {
            m_Instrumentors.push_back(std::move(instrumentor));
        }

        void BeginSession(const std::string& name, const std::string& filepath = "results.json");
        void EndSession();

        Scoped Scope(const std::string& name);
        FunctionScoped Function(std::source_location source);

        inline static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }

        void SetRecursionLimit(std::size_t limit) { m_RecursionLimit = limit; }

        Instrumentor(const Instrumentor&)            = delete;
        Instrumentor(Instrumentor&&)                 = delete;
        Instrumentor& operator=(const Instrumentor&) = delete;
        Instrumentor& operator=(Instrumentor&&)      = delete;
    private:
        Instrumentor();
        ~Instrumentor();

        std::size_t m_RecursionLimit = 100;
        std::string m_SessionName;
        std::string m_Filepath;
        std::vector<std::unique_ptr<BaseInstrumentor>> m_Instrumentors;
        std::unordered_map<std::string, std::size_t> m_Functions; // We want to automatically detect recursion
        JsonObject m_Json;
    };
} // namespace Nebula::Profiling
