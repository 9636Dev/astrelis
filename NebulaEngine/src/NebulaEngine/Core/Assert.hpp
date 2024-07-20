/**
 * @file Assert.hpp
 * Contains asserting macros. Only the client used macros are documented.
 */

#pragma once

#include "Debug.hpp"
#include "Log.hpp"

#ifdef NEBULA_DEBUG
    #define NEBULA_INTERNAL_BUILD_TYPE 0
    /**
     * @def NEBULA_ASSERT(x, message)
     * @brief Asserts that the expression is true, if not, it will log an error and break the debugger
     * This is only available in debug builds
     */
    #define NEBULA_ASSERT(x, ...)                                       \
        {                                                               \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */)   \
            {                                                           \
                NEBULA_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
                NEBULA_DEBUG_BREAK();                                   \
            }                                                           \
        }
    #define NEBULA_CORE_ASSERT(x, ...)                                       \
        {                                                                    \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */)        \
            {                                                                \
                NEBULA_CORE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
                NEBULA_DEBUG_BREAK();                                        \
            }                                                                \
        }
#else
    #define NEBULA_ASSERT(x, ...)
    #define NEBULA_CORE_ASSERT(x, ...)
#endif

#if defined(NEBULA_RELEASE) || NEBULA_INTERNAL_BUILD_TYPE <= 1
    #ifndef NEBULA_INTERNAL_BUILD_TYPE
        #define NEBULA_INTERNAL_BUILD_TYPE 1
    #endif

    /**
     * @def NEBULA_VERIFY(x, message)
     * @brief Verifies that the expression is true, if not, it will log an error and break the debugger
     * This is available in release (internal) and debug builds
     */
    #define NEBULA_VERIFY(x, ...)                                          \
        {                                                                  \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */)      \
            {                                                              \
                NEBULA_LOG_ERROR("Verification Failed: {0}", __VA_ARGS__); \
                NEBULA_DEBUG_BREAK();                                      \
            }                                                              \
        }
    #define NEBULA_CORE_VERIFY(x, ...)                                          \
        {                                                                       \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */)           \
            {                                                                   \
                NEBULA_CORE_LOG_ERROR("Verification Failed: {0}", __VA_ARGS__); \
                NEBULA_DEBUG_BREAK();                                           \
            }                                                                   \
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

    /**
     * @def NEBULA_REQUIRE(x, message)
     * @brief Requires that the expression is true, if not, it will log an error
     * This is available in all builds
     */
    #define NEBULA_REQUIRE(x, ...)                                        \
        {                                                                 \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */)     \
            {                                                             \
                NEBULA_LOG_ERROR("Requirement Failed: {0}", __VA_ARGS__); \
            }                                                             \
        }
    #define NEBULA_CORE_REQUIRE(x, ...)                                        \
        {                                                                      \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */)          \
            {                                                                  \
                NEBULA_CORE_LOG_ERROR("Requirement Failed: {0}", __VA_ARGS__); \
            }                                                                  \
        }
#endif


