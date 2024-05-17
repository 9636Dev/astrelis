#include "Instrumentor.hpp"

#include "../Log/Log.hpp"

#include <fstream>

namespace Nebula::Profiling
{
    Instrumentor::Instrumentor()
    {
#ifdef NEBULA_DEBUG
        NEB_CORE_LOG_WARN("Profiling is done in debug mode!");
#endif
    }

    Instrumentor::~Instrumentor()
    {
        if (!m_SessionName.empty())
        {
            EndSession();
        }
    }

    void Instrumentor::BeginSession(const std::string& name, const std::string& filepath)
    {
        if (!m_SessionName.empty())
        {
            NEB_CORE_LOG_WARN("Session '{}' already exists. Ending session before starting a new one.", m_SessionName);
            EndSession();
        }

        m_SessionName = name;
        m_Filepath    = filepath;
        m_Json        = JsonObject();
    }

    void Instrumentor::EndSession()
    {
        if (!m_SessionName.empty())
        {
            NEB_CORE_LOG_INFO("Ending session '{}'", m_SessionName);
            std::ofstream file(m_Filepath, std::ios::out | std::ios::trunc);
            if (!file.is_open())
            {
                NEB_CORE_LOG_ERROR("Failed to open file '{}'", m_Filepath);
                return;
            }
            file << m_Json.StringValue();
            file.close();

            m_SessionName.clear();
        }
        else
        {
            NEB_CORE_LOG_WARN("No session to end.");
        }
    }

    Instrumentor::Scoped Instrumentor::Scope(const std::string& name) { return Scoped(*this, name); }

    Instrumentor::FunctionScoped Instrumentor::Function(std::source_location source)
    {
        return FunctionScoped(*this, source);
    }

    Instrumentor::Scoped::Scoped(Instrumentor& instrumentor, const std::string& name) :
        m_Instrumentor(instrumentor),
        m_Name(name)
    {
        m_Scopes.reserve(m_Instrumentor.m_Instrumentors.size());
        for (auto& instrumentor : m_Instrumentor.m_Instrumentors)
        {
            m_Scopes.push_back(instrumentor->Scope());
        }
    }

    Instrumentor::FunctionScoped::FunctionScoped(Instrumentor& instrumentor, const std::source_location& function) :
        Scoped(instrumentor, function.function_name())
    {
        m_Instrumentor.m_Functions[function.function_name()]++;
    }

    void Instrumentor::Scoped::End()
    {
        if (m_Ended)
        {
            return;
        }
        JsonObject instruments;
        for (auto& scope : m_Scopes)
        {
            scope->End(instruments);
        }

        m_Instrumentor.m_Json[m_Name] = JsonValue(instruments);
    }

    void Instrumentor::FunctionScoped::PreDestroy()
    {
        for (auto& scope : m_Scopes)
        {
            scope->ActualEnd();
        }

        auto iter = m_Instrumentor.m_Functions.find(m_Name);
        if (iter == m_Instrumentor.m_Functions.end())
        {
            NEB_CORE_LOG_WARN("Function '{}' not found in m_Functions", m_Name);
            return;
        }

        if (iter->second-- > m_Instrumentor.m_RecursionLimit)
        {
            m_Ended = true;
            return;
        }

        if (iter->second == 0)
        {
            m_Instrumentor.m_Functions.erase(iter);
        }
        else
        {
            m_Name += " (recursive call: " + std::to_string(iter->second) + ")";
        }
    }

} // namespace Nebula::Profiling
