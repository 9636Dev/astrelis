#pragma once

#include "Platform.hpp"
#include "Renderer.hpp"

#include <Tracy/Tracy.hpp>

#ifdef ASTRELIS_PROFILE_GPU
    #define ASTRELIS_RUN_PROFILE_GPU(...) __VA_ARGS__

    // Tracy Vulkan is not supported well on MoltenVK
    #define ASTRELIS_PROFILE_VULKAN(...) ASTRELIS_RUN_PLATFORM_NOT_MACOS(__VA_ARGS__)
#else
    #define ASTRELIS_RUN_PROFILE_GPU(...)
    #define ASTRELIS_PROFILE_VULKAN(...)
#endif


#define ASTRELIS_PROFILE_FUNCTION()  ZoneScoped
#define ASTRELIS_PROFILE_SCOPE(name) ZoneScopedN(name)
#define ASTRELIS_PROFILE_END_FRAME() FrameMark
