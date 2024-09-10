#include "Fence.hpp"

#include "Astrelis/Core/Assert.hpp"
#include "Astrelis/Core/Log.hpp"

namespace Astrelis::Vulkan
{
    bool Fence::Init(LogicalDevice& device)
    {
        VkFenceCreateInfo fenceInfo {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateFence(device.GetHandle(), &fenceInfo, nullptr, &m_Fence) != VK_SUCCESS)
        {
            ASTRELIS_CORE_LOG_ERROR("Failed to create fence!");
            return false;
        }
        return true;
    }

    void Fence::Destroy(LogicalDevice& device)
    {
        ASTRELIS_CORE_ASSERT(m_Fence, "Fence is null!");
        vkDestroyFence(device.GetHandle(), m_Fence, nullptr);
        m_Fence = nullptr;
    }

    void Fence::Wait(LogicalDevice& device, std::uint64_t timeout)
    {
        ASTRELIS_CORE_ASSERT(m_Fence, "Fence is null!");
        auto res = vkWaitForFences(device.GetHandle(), 1, &m_Fence, VK_TRUE, timeout);
        (void)res;
        ASTRELIS_CORE_ASSERT(res == VK_SUCCESS, "Failed to wait for fence!");
    }

    void Fence::Reset(LogicalDevice& device)
    {
        ASTRELIS_CORE_ASSERT(m_Fence, "Fence is null!");
        ASTRELIS_CORE_ASSERT(vkGetFenceStatus(device.GetHandle(), m_Fence) == VK_SUCCESS,
                           "Fence is not signalled before reset!");
        auto res = vkResetFences(device.GetHandle(), 1, &m_Fence);
        (void)res;
        ASTRELIS_CORE_ASSERT(res == VK_SUCCESS, "Failed to reset fence!");
    }
} // namespace Astrelis::Vulkan
