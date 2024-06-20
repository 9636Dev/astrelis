#pragma once

#include "CommandBuffer.hpp"
#include "LogicalDevice.hpp"
#include "SwapChain.hpp"

#include <vulkan/vulkan.h>

namespace Nebula::Vulkan
{
    struct RenderPassInfo
    {
        VkAttachmentLoadOp loadOp          = VK_ATTACHMENT_LOAD_OP_CLEAR;
        VkAttachmentStoreOp storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        VkAttachmentLoadOp stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        VkAttachmentStoreOp stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        RenderPassInfo() = default;
    };

    class FrameBuffer;

    class RenderPass
    {
    public:
        RenderPass()                             = default;
        ~RenderPass()                            = default;
        RenderPass(const RenderPass&)            = delete;
        RenderPass& operator=(const RenderPass&) = delete;
        RenderPass(RenderPass&&)                 = delete;
        RenderPass& operator=(RenderPass&&)      = delete;

        [[nodiscard]] bool Init(LogicalDevice& device, SwapChain& swapChain, RenderPassInfo info = {});
        void Destroy(LogicalDevice& device);

        void Begin(CommandBuffer& commandBuffer, FrameBuffer& frameBuffer, VkExtent2D extent);
        void End(CommandBuffer& buffer);

        VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    };
} // namespace Nebula::Vulkan
