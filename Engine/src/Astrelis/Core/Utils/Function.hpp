#pragma once

template<typename... Args> constexpr inline void ASTRELIS_UNUSED([[maybe_unused]] Args&&... args) {}

/// @def ASTRELIS_ALWAYS_INLINE
/// @brief Forces the compiler to inline the function

/// @def ASTRELIS_NEVER_INLINE
/// @brief Forces the compiler to never inline the function
/// @note This is useful for debugging purposes

/// @def ASTRELIS_UNREACHABLE
/// @brief Informs the compiler that the code path is unreachable

// Optimization macros
#ifdef __clang__
    #define ASTRELIS_ALWAYS_INLINE __attribute__((always_inline))
    #define ASTRELIS_NEVER_INLINE  __attribute__((noinline))
    #define ASTRELIS_UNREACHABLE   __builtin_unreachable()
#elif _MSC_VER
    #define ASTRELIS_ALWAYS_INLINE __forceinline
    #define ASTRELIS_NEVER_INLINE  __declspec(noinline)
    #define ASTRELIS_UNREACHABLE   __assume(0)
#else
    #error "Compiler not supported"
#endif
