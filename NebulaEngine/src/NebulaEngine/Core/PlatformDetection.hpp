#pragma once

#ifdef _WIN32
    #define NEBULA_PLATFORM_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #define NEBULA_PLATFORM_IOS
        #error "IOS is not supported!"
    #elif TARGET_OS_MAC == 1
        #define NEBULA_PLATFORM_MACOS
    #else
        #error "Unknown Apple platform!"
    #endif
#elif defined(__ANDROID__)
    #define NEBULA_PLATFORM_ANDROID
    #error "Android is not supported!"
#elif defined(__linux__)
    #define NEBULA_PLATFORM_LINUX
#else
    #error "Unknown platform!"
#endif
