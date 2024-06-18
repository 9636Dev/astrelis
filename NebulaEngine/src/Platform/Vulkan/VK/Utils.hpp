#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "NebulaEngine/Core/PlatformDetection.hpp"

namespace Nebula::Vulkan
{
    inline const std::vector<const char*>& GetValidationLayers()
    {
        static const std::vector<const char*> layers = {
            "VK_LAYER_KHRONOS_validation",
        };

        return layers;
    }

    inline const std::vector<const char*>& GetDeviceExtensions()
    {
        static const std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME,
#ifdef NEBULA_PLATFORM_MACOS
                                                            "VK_KHR_portability_subset"
#endif
        };

        return extensions;
    }

    bool CheckValidationLayerSupport();
    std::vector<const char*> GetRequiredExtensions(bool enableValidationLayers);
} // namespace Nebula::Vulkan
