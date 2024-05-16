#include "Instrumentor.hpp"

#include "../Log/Log.hpp"
#include "NebulaCore/Util/Assert.hpp"

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
        m_Json = JsonObject();
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

    Instrumentor::Scoped Instrumentor::Scope(std::string name) {
        if (m_ScopeNames.find(name) != m_ScopeNames.end())
        {
            m_ScopeNames[name]++;
            name += " - RD_" + std::to_string(m_ScopeNames[name]); // RD = Recursive Depth
        }
        else
        {
            m_ScopeNames[name] = 0;
        }

        return Scoped(*this, name);
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

    Instrumentor::Scoped::~Scoped()
    {
        JsonObject instruments;
        for (auto& scope : m_Scopes)
        {
            scope->End(instruments);
        }

        m_Instrumentor.m_Json[m_Name] = JsonValue(instruments);

        // find the part - RD_# and remove it
        auto pos = m_Name.find(" - RD_");
        if (pos != std::string::npos)
        {
            m_Name = m_Name.substr(0, pos);
        }

        auto iter = m_Instrumentor.m_ScopeNames.find(m_Name);
        NEBULA_CORE_VERIFY(iter != m_Instrumentor.m_ScopeNames.end(), "Scope name not found in map");
        if (iter->second == 0)
        {
            m_Instrumentor.m_ScopeNames.erase(iter);
        }
        else
        {
            iter->second--;
        }
    }

} // namespace Nebula::Profiling
