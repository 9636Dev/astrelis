/**
 * @file Assert.hpp
 * Contains asserting macros. Only the client used macros are documented.
 */

#pragma once

#include "Astrelis/Core/Log.hpp"

#include "Debug.hpp"

#ifdef ASTRELIS_DEBUG
    #define ASTRELIS_INTERNAL_BUILD_TYPE 0
    /**
     * @def ASTRELIS_ASSERT(x, message)
     * @brief Asserts that the expression is true, if not, it will log an error and break the debugger
     * This is only available in debug builds
     */
    #define ASTRELIS_ASSERT(x, ...) \
        { \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */) { \
                ASTRELIS_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
                ASTRELIS_DEBUG_BREAK(); \
            } \
        }
    #define ASTRELIS_CORE_ASSERT(x, ...) \
        { \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */) { \
                ASTRELIS_CORE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
                ASTRELIS_DEBUG_BREAK(); \
            } \
        }

    #define ASTRELIS_CORE_ASSERTN(x, ...) \
        { \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */) { \
                ASTRELIS_CORE_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            } \
        }
#else
    #define ASTRELIS_ASSERT(x, ...) (void)(sizeof(x))
    #define ASTRELIS_CORE_ASSERT(x, ...) (void)(sizeof(x))
    #define ASTRELIS_CORE_ASSERTN(x, ...) (void)(sizeof(x))
#endif

#if defined(ASTRELIS_RELEASE) || ASTRELIS_INTERNAL_BUILD_TYPE <= 1
    #ifndef ASTRELIS_INTERNAL_BUILD_TYPE
        #define ASTRELIS_INTERNAL_BUILD_TYPE 1
    #endif

    /**
     * @def ASTRELIS_VERIFY(x, message)
     * @brief Verifies that the expression is true, if not, it will log an error and break the debugger
     * This is available in release (internal) and debug builds
     */
    #define ASTRELIS_VERIFY(x, ...) \
        { \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */) { \
                ASTRELIS_LOG_ERROR("Verification Failed: {0}", __VA_ARGS__); \
                ASTRELIS_DEBUG_BREAK(); \
            } \
        }
    #define ASTRELIS_CORE_VERIFY(x, ...) \
        { \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */) { \
                ASTRELIS_CORE_LOG_ERROR("Verification Failed: {0}", __VA_ARGS__); \
                ASTRELIS_DEBUG_BREAK(); \
            } \
        }
    #define ASTRELIS_CORE_VERIFYN(x, ...) \
        { \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */) { \
                ASTRELIS_CORE_LOG_ERROR("Verification Failed: {0}", __VA_ARGS__); \
            } \
        }
#else
    #define ASTRELIS_VERIFY(x, ...) (void)(sizeof(x))
    #define ASTRELIS_CORE_VERIFY(x, ...) (void)(sizeof(x))
    #define ASTRELIS_CORE_VERIFYN(x, ...) (void)(sizeof(x))
#endif

#if defined(ASTRELIS_RELEASE) || ASTRELIS_INTERNAL_BUILD_TYPE <= 2
    #ifndef ASTRELIS_INTERNAL_BUILD_TYPE
        #define ASTRELIS_INTERNAL_BUILD_TYPE 2
    #endif

    /**
     * @def ASTRELIS_REQUIRE(x, message)
     * @brief Requires that the expression is true, if not, it will log an error
     * This is available in all builds
     */
    #define ASTRELIS_REQUIRE(x, ...) \
        { \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */) { \
                ASTRELIS_LOG_ERROR("Requirement Failed: {0}", __VA_ARGS__); \
            } \
        }
    #define ASTRELIS_CORE_REQUIRE(x, ...) \
        { \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */) { \
                ASTRELIS_CORE_LOG_ERROR("Requirement Failed: {0}", __VA_ARGS__); \
            } \
        }

    #define ASTRELIS_CORE_REQUIREN(x, ...) \
        { \
            if (!(x) /* NOLINT(readability-simplify-boolean-expr) */) { \
                ASTRELIS_CORE_LOG_ERROR("Requirement Failed: {0}", __VA_ARGS__); \
            } \
        }
#endif
