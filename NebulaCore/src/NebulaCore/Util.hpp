#pragma once

#include "Core.hpp"

#ifdef NEBULA_PLATFORM_WINDOWS
    #define NEBULA_FORCEINLINE                       __forceinline
    #define NEBULA_NOINLINE                          __declspec(noinline)
    #define NEBULA_NORETURN                          __declspec(noreturn)
    #define NEBULA_PLATFORM_SHARED_LIBRARY_EXTENSION ".dll"
#elif defined(NEBULA_PLATFORM_LINUX) || defined(NEBULA_PLATFORM_APPLE)
    #define NEBULA_FORCEINLINE inline __attribute__((always_inline))
    #define NEBULA_NOINLINE    __attribute__((noinline))
    #define NEBULA_NORETURN    __attribute__((noreturn))
    #ifdef NEBULA_PLATFORM_APPLE
        #define NEBULA_PLATFORM_SHARED_LIBRARY_EXTENSION ".dylib"
    #else
        #define NEBULA_PLATFORM_SHARED_LIBRARY_EXTENSION ".so"
    #endif
#endif

// We set the debug break for the different compilers or use signals for Linux
// MSVC
#ifdef NEBULA_USE_DEBUGBREAK_SIGNAL
    #include <signal.h>
    #define NEB_DEBUGBREAK() raise(SIGTRAP)
#else
    #ifdef _MSC_VER
        #define NEB_DEBUGBREAK() __debugbreak()
    #elif defined(__GNUC__) || defined(__clang__)
        #define NEB_DEBUGBREAK() __builtin_trap()
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
#endif

#ifdef NEBULA_DEBUG
    #define NEB_RUN_IF_DEBUG(x) x
#else
    #define NEB_RUN_IF_DEBUG(x)
#endif
