#pragma once

#ifdef NEBULA_ENABLE_PROFILING
    #include "Instrumentor.hpp"
    #include <source_location>

    #define NEBULA_GET_INSTRUMENTOR()                     ::Nebula::Profiling::Instrumentor::Get()
    #define NEBULA_PROFILE_BEGIN_SESSION(name, filepath) NEBULA_GET_INSTRUMENTOR().BeginSession(name, filepath)
    #define NEBULA_PROFILE_END_SESSION()                 NEBULA_GET_INSTRUMENTOR().EndSession()
    #define NEBULA_PROFILE_ADD_INSTRUMENTORS() { \
        NEBULA_GET_INSTRUMENTOR().AddInstrumentor(std::make_unique<::Nebula::Profiling::TimerInstrumentor>()); \
    }

    #define NEBULA_PROFILE_SCOPE(name)                   auto timer##__LINE__ = NEBULA_GET_INSTRUMENTOR().Scope(name)
    #define NEBULA_PROFILE_FUNCTION()                    NEBULA_PROFILE_SCOPE(std::source_location::current().function_name())
#else
    #define NEBULA_PROFILE_BEGIN_SESSION(name, filepath)
    #define NEBULA_PROFILE_END_SESSION()
    #define NEBULA_PROFILE_ADD_INSTRUMENTORS()

    #define NEBULA_PROFILE_SCOPE(name)
    #define NEBULA_PROFILE_FUNCTION()
#endif

