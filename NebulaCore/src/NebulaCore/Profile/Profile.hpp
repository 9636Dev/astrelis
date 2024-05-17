#pragma once

/**
* @file Profile.hpp
* @brief A file that contains macros for profiling
*/

/**
* @def NEBULA_PROFILE_BEGIN_SESSION(name, filepath)
* @brief Begins a profiling session
* @param name The name of the session
* @param filepath The filepath to save the results to
*/

/**
* @def NEBULA_PROFILE_END_SESSION()
* @brief Ends the current profiling session
*/

/**
* @def NEBULA_PROFILE_ADD_INSTRUMENTORS()
* @brief Adds the default instrumentors
*/

/**
* @def NEBULA_PROFILE_RECURSION_LIMIT(limit)
* @brief Sets the recursion limit (1 index based)
* @param limit The recursion limit
*/

/**
* @def NEBULA_PROFILE_SCOPE(name)
* @brief Creates a scoped section of code
* @param name The name of the section
*/

/**
* @def NEBULA_PROFILE_FUNCTION()
* @brief Creates a scoped function, used to profile a function
*/

#ifdef NEBULA_ENABLE_PROFILING
    #ifndef NEBULA_CORE_INTERNAL
        #include "Instrumentor.hpp"
        #include <source_location>
    #endif

    #define NEBULA_GET_INSTRUMENTOR()                    ::Nebula::Profiling::Instrumentor::Get()
    #define NEBULA_PROFILE_BEGIN_SESSION(name, filepath) NEBULA_GET_INSTRUMENTOR().BeginSession(name, filepath)
    #define NEBULA_PROFILE_END_SESSION()                 NEBULA_GET_INSTRUMENTOR().EndSession()
    #define NEBULA_PROFILE_ADD_INSTRUMENTORS()                                                                     \
        {                                                                                                          \
            NEBULA_GET_INSTRUMENTOR().AddInstrumentor(std::make_unique<::Nebula::Profiling::TimerInstrumentor>()); \
        }
    #define NEBULA_PROFILE_RECURSION_LIMIT(limit) NEBULA_GET_INSTRUMENTOR().SetRecursionLimit(limit)

    #define NEBULA_PROFILE_SCOPE(name)            auto timer##__LINE__ = NEBULA_GET_INSTRUMENTOR().Scope(name)
    #define NEBULA_PROFILE_FUNCTION() \
        auto timer##__LINE__ = NEBULA_GET_INSTRUMENTOR().Function(std::source_location::current())
#else
    #define NEBULA_PROFILE_BEGIN_SESSION(name, filepath)
    #define NEBULA_PROFILE_END_SESSION()
    #define NEBULA_PROFILE_ADD_INSTRUMENTORS()
    #define NEBULA_PROFILE_RECURSION_LIMIT(limit)

    #define NEBULA_PROFILE_SCOPE(name)
    #define NEBULA_PROFILE_FUNCTION()
#endif

