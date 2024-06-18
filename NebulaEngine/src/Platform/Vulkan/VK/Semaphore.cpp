#include "Semaphore.hpp"

#include "NebulaEngine/Core/Log.hpp"

namespace Nebula::Vulkan
{
    bool Semaphore::Init(LogicalDevice& device)
    {
        VkSemaphoreCreateInfo semaphoreInfo {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        if (vkCreateSemaphore(device.GetHandle(), &semaphoreInfo, nullptr, &m_Semaphore) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create semaphore!");
            return false;
        }
        return true;
    }

    void Semaphore::Destroy(LogicalDevice& device) { vkDestroySemaphore(device.GetHandle(), m_Semaphore, nullptr); }
} // namespace Nebula::Vulkan
