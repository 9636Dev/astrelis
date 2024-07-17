#pragma once

template<typename... Args> constexpr inline void NEBULA_UNUSED([[maybe_unused]] Args&&... args) {}

// Optimization macros
#ifdef __clang__
    #define NEBULA_ALWAYS_INLINE __attribute__((always_inline))
    #define NEBULA_NEVER_INLINE  __attribute__((noinline))
    #define NEBULA_UNREACHABLE   __builtin_unreachable()
#elif _MSC_VER
    //  TODO: implement
    #define NEBULA_ALWAYS_INLINE
    #define NEBULA_NEVER_INLINE
    #define NEBULA_UNREACHABLE
#else
    #error "Compiler not supported"
#endif
