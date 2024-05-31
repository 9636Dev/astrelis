#pragma once

#ifdef NEBULA_DEBUG
    #ifdef NEBULA_PLATFORM_WINDOWS
        #define NEBULA_DEBUG_BREAK() __debugbreak()
    #elif defined(NEBULA_PLATFORM_LINUX)
        #define NEBULA_DEBUG_BREAK() __builtin_debugtrap()
    #elif defined(NEBULA_PLATFORM_MACOS)
        #define NEBULA_DEBUG_BREAK() __builtin_trap()
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
#else
    #define NEBULA_DEBUG_BREAK()
#endif
