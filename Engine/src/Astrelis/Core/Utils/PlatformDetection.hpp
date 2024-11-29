#pragma once

/// @file PlatformDetection.hpp
/// @brief Contains macros to detect the platform the code is running on.
/// @details This file contains macros to detect the platform the code is running on, and define the platform for the rest of the code.

/// @def ASTRELIS_PLATFORM_WINDOWS
/// @brief Defined if the platform is Windows

/// @def ASTRELIS_PLATFORM_MACOS
/// @brief Defined if the platform is MacOS

/// @def ASTRELIS_PLATFORM_LINUX
/// @brief Defined if the platform is Linux

#ifdef _WIN32
    #define ASTRELIS_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #define ASTRELIS_PLATFORM_IOS
        #error "IOS is not supported!"
    #elif TARGET_OS_MAC == 1
        #define ASTRELIS_PLATFORM_MACOS
    #else
        #error "Unknown Apple platform!"
    #endif
#elif defined(__ANDROID__)
    #define ASTRELIS_PLATFORM_ANDROID
    #error "Android is not supported!"
#elif defined(__linux__)
    #define ASTRELIS_PLATFORM_LINUX
#else
    #error "Unknown platform!"
#endif
