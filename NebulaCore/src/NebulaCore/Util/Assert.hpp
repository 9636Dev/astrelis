#pragma once

#include <iostream>

#include "Debug.hpp"

#ifdef NEBULA_DEBUG
    #define NEBULA_INTERNAL_BUILD_TYPE 0
    #define NEBULA_ASSERT(x, ...)                                   \
        {                                                           \
            if (!(x))                                               \
            {                                                       \
                NEB_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
                NEBULA_DEBUG_BREAK();                               \
            }                                                       \
        }
    #define NEBULA_CORE_ASSERT(x, ...)                                   \
        {                                                                \
            if (!(x))                                                    \
            {                                                            \
                NEB_CORE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
                NEBULA_DEBUG_BREAK();                                    \
            }                                                            \
        }
    #define NEBULA_INTERNAL_ASSERT(x, ...)                                     \
        {                                                                      \
            if (!(x))                                                          \
            {                                                                  \
                std::cerr << "Assertion Failed: " << __VA_ARGS__ << std::endl; \
                NEBULA_DEBUG_BREAK();                                          \
            }                                                                  \
        }
#else
    #define NEBULA_ASSERT(x, ...)
    #define NEBULA_CORE_ASSERT(x, ...)
    #define NEBULA_INTERNAL_ASSERT(x, ...)
#endif

#if defined(NEBULA_RELEASE) || NEBULA_INTERNAL_BUILD_TYPE <= 1
    #ifndef NEBULA_INTERNAL_BUILD_TYPE
        #define NEBULA_INTERNAL_BUILD_TYPE 1
    #endif

    #define NEBULA_VERIFY(x, ...)                                      \
        {                                                              \
            if (!(x))                                                  \
            {                                                          \
                NEB_LOG_ERROR("Verification Failed: {0}", __VA_ARGS__); \
                NEBULA_DEBUG_BREAK();                                  \
            }                                                          \
        }
    #define NEBULA_CORE_VERIFY(x, ...)                                      \
        {                                                                   \
            if (!(x))                                                       \
            {                                                               \
                NEB_CORE_LOG_ERROR("Verification Failed: {0}", __VA_ARGS__); \
                NEBULA_DEBUG_BREAK();                                       \
            }                                                               \
        }
#else
    #define NEBULA_VERIFY(x, ...)      x
    #define NEBULA_CORE_VERIFY(x, ...) x
#endif

#if defined(NEBULA_RELEASE) || NEBULA_INTERNAL_BUILD_TYPE <= 2
    #ifndef NEBULA_INTERNAL_BUILD_TYPE
        #define NEBULA_INTERNAL_BUILD_TYPE 2
    #endif

    // TODO(9636Dev): Requirement will throw a Nebula::RequirementFailed exception
    #define NEBULA_REQUIRE(x, ...)                                    \
        {                                                             \
            if (!(x))                                                 \
            {                                                         \
                NEB_LOG_ERROR("Requirement Failed: {0}", __VA_ARGS__); \
            }                                                         \
        }
    #define NEBULA_CORE_REQUIRE(x, ...)                                    \
        {                                                                  \
            if (!(x))                                                      \
            {                                                              \
                NEB_CORE_LOG_ERROR("Requirement Failed: {0}", __VA_ARGS__); \
                ;                                                          \
            }                                                              \
        }
#endif
