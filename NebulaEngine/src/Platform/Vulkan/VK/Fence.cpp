#include "Fence.hpp"

#include "NebulaEngine/Core/Log.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

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

    void Fence::Destroy(LogicalDevice& device) { vkDestroyFence(device.GetHandle(), m_Fence, nullptr); }

    void Fence::Wait(RefPtr<GraphicsContext>& context, std::uint64_t timeout)
    {
        vkWaitForFences(context.As<VulkanGraphicsContext>()->m_LogicalDevice.GetHandle(), 1, &m_Fence, VK_TRUE, timeout);
    }

    void Fence::Reset(RefPtr<GraphicsContext>& context)
    {
        vkResetFences(context.As<VulkanGraphicsContext>()->m_LogicalDevice.GetHandle(), 1, &m_Fence);
    }
} // namespace Nebula::Vulkan
