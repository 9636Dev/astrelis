#include "Fence.hpp"

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Core/Log.hpp"

namespace Nebula::Vulkan
{
    bool Fence::Init(LogicalDevice& device)
    {
        VkFenceCreateInfo fenceInfo {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateFence(device.GetHandle(), &fenceInfo, nullptr, &m_Fence) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create fence!");
            return false;
        }
        return true;
    }

    void Fence::Destroy(LogicalDevice& device)
    {
        NEBULA_CORE_ASSERT(m_Fence, "Fence is null!");
        vkDestroyFence(device.GetHandle(), m_Fence, nullptr);
        m_Fence = nullptr;
    }

    void Fence::Wait(LogicalDevice& device, std::uint64_t timeout)
    {
        NEBULA_CORE_ASSERT(m_Fence, "Fence is null!");
        NEBULA_CORE_ASSERT(vkWaitForFences(device.GetHandle(), 1,
                                           &m_Fence, VK_TRUE, timeout) == VK_SUCCESS,
                           "Failed to wait for fence!");
    }

    void Fence::Reset(LogicalDevice& device)
    {
        NEBULA_CORE_ASSERT(m_Fence, "Fence is null!");
        NEBULA_CORE_ASSERT(vkGetFenceStatus(device.GetHandle(), m_Fence) == VK_SUCCESS, "Fence is not signalled before reset!");
        NEBULA_CORE_ASSERT(
            vkResetFences(device.GetHandle(), 1, &m_Fence) == VK_SUCCESS,
            "Failed to reset fence!");
    }
} // namespace Nebula::Vulkan
