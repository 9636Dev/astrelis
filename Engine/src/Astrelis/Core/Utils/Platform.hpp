#pragma once

#include "PlatformDetection.hpp"

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
