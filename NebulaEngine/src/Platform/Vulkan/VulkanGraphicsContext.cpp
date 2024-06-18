#include "VulkanGraphicsContext.hpp"

#include "NebulaEngine/Core/Log.hpp"
#include "VK/Utils.hpp"
#include "VK/VulkanExt.hpp"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace Nebula
{
    VulkanGraphicsContext::VulkanGraphicsContext(RawRef<GLFWwindow*> window) : m_Window(std::move(window)) {}

    VulkanGraphicsContext::~VulkanGraphicsContext() = default;

    bool VulkanGraphicsContext::Init()
    {
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define INIT_COMPONENT(...) \
    if (!__VA_ARGS__)       \
    return false

        if (m_IsInitialized)
        {
            NEBULA_CORE_LOG_WARN("Vulkan Graphics Context already initialized!");
            return false;
        }

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
        if (m_Debug)
        {
            if (!Vulkan::CheckValidationLayerSupport())
            {
                NEBULA_CORE_LOG_ERROR("Validation layers requested, but not available!");
                return false;
            }

            Vulkan::PopulateDebugMessengerCreateInfo(debugCreateInfo);
        }

        INIT_COMPONENT(m_Instance.Init("Nebula", "Nebula Engine", Vulkan::APIVersion(1, 0),
                                       Vulkan::GetRequiredExtensions(m_Debug), Vulkan::GetValidationLayers(),
                                       &debugCreateInfo));
        m_PhysicalDevice.PickBestDevice(m_Instance);
        INIT_COMPONENT(m_PhysicalDevice.IsFound());
        INIT_COMPONENT(m_Surface.Init(m_Instance, m_Window));
        INIT_COMPONENT(m_LogicalDevice.Init(m_PhysicalDevice, m_Surface, Vulkan::GetDeviceExtensions(),
                                            m_Debug ? Vulkan::GetValidationLayers() : std::vector<const char*>()));
        INIT_COMPONENT(m_SwapChain.Init(m_PhysicalDevice, m_LogicalDevice, m_Surface));

        NEBULA_CORE_LOG_INFO("Vulkan Graphics Context initialized!");
        m_IsInitialized = true;
        return true;
#undef INIT_COMPONENT
    }

    void VulkanGraphicsContext::Shutdown()
    {
        m_SwapChain.Destroy(m_LogicalDevice);
        m_LogicalDevice.Destroy();
        m_Surface.Destroy(m_Instance);
        // Physical device doesn't need to be destroyed
        m_Instance.Destroy();
    }

    RefPtr<VulkanGraphicsContext> VulkanGraphicsContext::Create(RawRef<GLFWwindow*> window)
    {
        return RefPtr<VulkanGraphicsContext>::Create(window);
    }
} // namespace Nebula
