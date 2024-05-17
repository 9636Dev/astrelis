#pragma once

#ifdef NEBULA_PLATFORM_WINDOWS
#elif defined(NEBULA_PLATFORM_LINUX)
    #define NEBULA_API_IMPORT
    #define NEBULA_API_EXPORT __attribute__((visibility("default")))
#elif defined(NEBULA_PLATFORM_MACOS)
    #define NEBULA_API_IMPORT
    #define NEBULA_API_EXPORT __attribute__((visibility("default")))
#endif
