#pragma once

#include <Tracy/Tracy.hpp>

#include "Platform.hpp"
#include "Renderer.hpp"

#ifdef ASTRELIS_PROFILE_GPU
    #define ASTRELIS_RUN_PROFILE_GPU(...) __VA_ARGS__

    // Tracy Vulkan is not supported well on MoltenVK
    #define ASTRELIS_PROFILE_VULKAN(...) ASTRELIS_RUN_PLATFORM_NOT_MACOS(__VA_ARGS__)
#else
    #define ASTRELIS_RUN_PROFILE_GPU(...)
    #define ASTRELIS_PROFILE_VULKAN(...)
#endif


/// @def ASTRELIS_PROFILE_FUNCTION()
/// @brief A wrapper for Tracy's ZoneScoped
#define ASTRELIS_PROFILE_FUNCTION() ZoneScoped
/// @def ASTRELIS_PROFILE_SCOPE(name)
/// @brief A wrapper for Tracy's ZoneScopedN
#define ASTRELIS_PROFILE_SCOPE(name) ZoneScopedN(name)
/// @def ASTRELIS_PROFILE_GPU_SCOPE(name)
/// @brief A wrapper for Tracy's FrameMark
#define ASTRELIS_PROFILE_END_FRAME() FrameMark
