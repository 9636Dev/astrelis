#pragma once

#include "PlatformDetection.hpp"

/// @def ASTRELIS_RUN_PLATFORM_WINDOWS(...)
/// @brief Runs the code only if the platform is Windows
/// @param ... The code to run

/// @def ASTRELIS_RUN_PLATFORM_LINUX(...)
/// @brief Runs the code only if the platform is Linux
/// @param ... The code to run

/// @def ASTRELIS_RUN_PLATFORM_MACOS(...)
/// @brief Runs the code only if the platform is MacOS
/// @param ... The code to run

/// @def ASTRELIS_RUN_PLATFORM_NOT_WINDOWS(...)
/// @brief Runs the code only if the platform is not Windows
/// @param ... The code to run

/// @def ASTRELIS_RUN_PLATFORM_NOT_LINUX(...)
/// @brief Runs the code only if the platform is not Linux
/// @param ... The code to run

/// @def ASTRELIS_RUN_PLATFORM_NOT_MACOS(...)
/// @brief Runs the code only if the platform is not MacOS
/// @param ... The code to run

#ifdef ASTRELIS_PLATFORM_MACOS
    #define ASTRELIS_RUN_PLATFORM_MACOS(...) __VA_ARGS__
#else
    #define ASTRELIS_RUN_PLATFORM_MACOS(...)
#endif

#ifdef ASTRELIS_PLATFORM_WINDOWS
    #define ASTRELIS_RUN_PLATFORM_WINDOWS(...) __VA_ARGS__
#else
    #define ASTRELIS_RUN_PLATFORM_WINDOWS(...)
#endif

#ifdef ASTRELIS_PLATFORM_LINUX
    #define ASTRELIS_RUN_PLATFORM_LINUX(...) __VA_ARGS__
#else
    #define ASTRELIS_RUN_PLATFORM_LINUX(...)
#endif

#ifndef ASTRELIS_PLATFORM_MACOS
    #define ASTRELIS_RUN_PLATFORM_NOT_MACOS(...) __VA_ARGS__
#else
    #define ASTRELIS_RUN_PLATFORM_NOT_MACOS(...)
#endif

#ifndef ASTRELIS_PLATFORM_WINDOWS
    #define ASTRELIS_RUN_PLATFORM_NOT_WINDOWS(...) __VA_ARGS__
#else
    #define ASTRELIS_RUN_PLATFORM_NOT_WINDOWS(...)
#endif

#ifndef ASTRELIS_PLATFORM_LINUX
    #define ASTRELIS_RUN_PLATFORM_NOT_LINUX(...) __VA_ARGS__
#else
    #define ASTRELIS_RUN_PLATFORM_NOT_LINUX(...)
#endif
