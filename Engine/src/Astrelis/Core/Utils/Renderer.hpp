#pragma once

#ifdef ASTRELIS_RENDERER_VULKAN
    #define ASTRELIS_RUN_RENDERER_VULKAN(...) __VA_ARGS__
#else
    #define ASTRELIS_RUN_RENDERER_VULKAN(...) nullptr
#endif
