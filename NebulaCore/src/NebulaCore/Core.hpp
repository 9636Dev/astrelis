#pragma once

#ifdef _WINDOWS
#define NEBULA_PLATFORM_WINDOWS
#elif defined(__linux__)
#define NEBULA_PLATFORM_LINUX
#elif defined(__APPLE__)
#define NEBULA_PLATFORM_APPLE
#else
// Currently no mobile support
#error "Unknown platform"
#endif

#ifdef NEBULA_PLATFORM_WINDOWS
#define NEBULA_API_EXPORT __declspec(dllexport)
#define NEBULA_API_IMPORT __declspec(dllimport)
#elif defined(NEBULA_PLATFORM_LINUX) || defined(NEBULA_PLATFORM_APPLE)
#define NEBULA_API_EXPORT __attribute__((visibility("default")))
#define NEBULA_API_IMPORT
#endif

#ifdef NEBULA_EXPORT_DLL
    #define NEBULA_CORE_API NEBULA_API_EXPORT
#else
    #define NEBULA_CORE_API NEBULA_API_IMPORT
#endif
