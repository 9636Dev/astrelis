#pragma once

#include "Util.hpp"

#if defined(NEBULA_DEBUG) || defined(NEBULA_ENABLE_ASSERTS)
    #ifndef NEBULA_ASSERT_ONFAIL
        #ifndef NEBULA_NO_DEBUGBREAK
            #define NEBULA_ASSERT_ONFAIL NEB_DEBUGBREAK()
        #else
            #define NEBULA_ASSERT_ONFAIL
        #endif
    #endif

    #ifdef NEB_INCLUDE_LIBC_ASSERT
        // We don't use NEBULA_LOG here
        #include <iostream>

        #define NEB_ASSERT(x, ...)                                            \
            {                                                                 \
                if (!(x))                                                     \
                {                                                             \
                    std::cerr << "Assertion Failed: " << __VA_ARGS__ << "\n"; \
                    NEBULA_ASSERT_ONFAIL;                                     \
                }                                                             \
            }
    #else
        #include "Log.hpp"

        #define NEB_ASSERT(x, ...)                   \
            {                                        \
                if (!(x))                            \
                {                                    \
                    NEB_CORE_LOG_FATAL(__VA_ARGS__); \
                    NEBULA_ASSERT_ONFAIL;            \
                }                                    \
            }

    #endif
#else
    #define NEB_ASSERT(x, ...)
#endif
