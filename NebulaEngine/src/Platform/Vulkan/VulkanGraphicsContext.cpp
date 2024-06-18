#include "VulkanGraphicsContext.hpp"

#include "NebulaEngine/Core/Log.hpp"
#include "Platform/Vulkan/VK/VulkanExt.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#ifdef __APPLE__
    #include <vulkan/vulkan_macos.h>
#endif

namespace Nebula
{
    static const std::vector<const char*>& GetValidationLayers()
    {
        static const std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation",
        };

        return validationLayers;
    }

    bool CheckValidationLayerSupport()
    {
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : GetValidationLayers())
        {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers)
            {
                if (std::strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                return false;
            }
        }

        return true;
    }

    std::vector<const char*> GetRequiredExtensions(bool enableValidationLayers)
    {
        std::uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions      = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers)
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
#ifdef __APPLE__
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
        extensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#endif

        return extensions;
    }

    VulkanGraphicsContext::VulkanGraphicsContext()  = default;
    VulkanGraphicsContext::~VulkanGraphicsContext() = default;

    bool VulkanGraphicsContext::Init()
    {
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
        if (m_Debug)
        {
            if (!CheckValidationLayerSupport())
            {
                NEBULA_CORE_LOG_ERROR("Validation layers requested, but not available!");
                return false;
            }

            PopulateDebugMessengerCreateInfo(debugCreateInfo);
        }

        m_Instance.Init("Nebula", "Nebula Engine", GetRequiredExtensions(m_Debug), GetValidationLayers(),&debugCreateInfo);
        return true;
    }

    void VulkanGraphicsContext::Shutdown()
    {
        m_Instance.Destroy();
    }

    RefPtr<VulkanGraphicsContext> VulkanGraphicsContext::Create()
    {
        return RefPtr<VulkanGraphicsContext>::Create();
    }
} // namespace Nebula
