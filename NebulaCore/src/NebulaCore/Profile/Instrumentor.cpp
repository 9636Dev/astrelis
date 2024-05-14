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
        m_Sstream.str("");
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
            file << m_Sstream.str();
            file.close();

            m_SessionName.clear();
        }
        else
        {
            NEB_CORE_LOG_WARN("No session to end.");
        }
    }

    Instrumentor::Scoped Instrumentor::Scope(const std::string& name) { return Scoped(*this, name); }

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

    Instrumentor::Scoped::~Scoped()
    {
        std::stringstream sstream;
        sstream << m_Name << ":\n";
        for (auto& scope : m_Scopes)
        {
            scope->End(sstream);
        }

        m_Instrumentor.m_Sstream << sstream.str();
    }

} // namespace Nebula::Profiling
