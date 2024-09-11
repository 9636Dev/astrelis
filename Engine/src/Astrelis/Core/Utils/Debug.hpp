#pragma once

#include "PlatformDetection.hpp"

#ifdef ASTRELIS_DEBUG
    #ifdef ASTRELIS_PLATFORM_WINDOWS
        #define ASTRELIS_DEBUG_BREAK() __debugbreak()
    #elif defined(ASTRELIS_PLATFORM_LINUX)
        #define ASTRELIS_DEBUG_BREAK() __builtin_debugtrap()
    #elif defined(ASTRELIS_PLATFORM_MACOS)
        #define ASTRELIS_DEBUG_BREAK() __builtin_trap()
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
#else
    #define ASTRELIS_DEBUG_BREAK()
#endif
