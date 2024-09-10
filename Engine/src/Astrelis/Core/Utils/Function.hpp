#pragma once

template<typename... Args> constexpr inline void ASTRELIS_UNUSED([[maybe_unused]] Args&&... args) {}

// Optimization macros
#ifdef __clang__
    #define ASTRELIS_ALWAYS_INLINE __attribute__((always_inline))
    #define ASTRELIS_NEVER_INLINE  __attribute__((noinline))
    #define ASTRELIS_UNREACHABLE   __builtin_unreachable()
#elif _MSC_VER
    //  TODO: implement
    #define ASTRELIS_ALWAYS_INLINE
    #define ASTRELIS_NEVER_INLINE
    #define ASTRELIS_UNREACHABLE
#else
    #error "Compiler not supported"
#endif
