#pragma once

/// @def ASTRELIS_RUN_RENDERER_VULKAN(...)
/// @brief Runs the code only if the renderer is Vulkan
/// @param ... The code to run

#ifdef ASTRELIS_RENDERER_VULKAN
    #define ASTRELIS_RUN_RENDERER_VULKAN(...) __VA_ARGS__
#else
    #define ASTRELIS_RUN_RENDERER_VULKAN(...) nullptr
#endif
