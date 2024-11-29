#pragma once

#include <vulkan/vulkan.h>

#include "CommandBuffer.hpp"
#include "LogicalDevice.hpp"

namespace Astrelis::Vulkan {
    struct RenderSubpass {
        VkPipelineBindPoint                PipelineBindPoint;
        std::vector<VkAttachmentReference> ColorAttachments;
        VkAttachmentReference              DepthAttachment;
        std::vector<VkAttachmentReference> ResolveAttachments;

        RenderSubpass(VkPipelineBindPoint      bindPoint,
            std::vector<VkAttachmentReference> colorAttachments,
            VkAttachmentReference              depthAttachment,
            std::vector<VkAttachmentReference> resolveAttachments)
            : PipelineBindPoint(bindPoint), ColorAttachments(std::move(colorAttachments)),
              DepthAttachment(depthAttachment), ResolveAttachments(std::move(resolveAttachments)) {
        }

        RenderSubpass(
            VkPipelineBindPoint bindPoint, std::vector<VkAttachmentReference> colorAttachments)
            : PipelineBindPoint(bindPoint), ColorAttachments(std::move(colorAttachments)),
              DepthAttachment({}) {
            DepthAttachment.attachment = VK_ATTACHMENT_UNUSED;
        }
    };

    struct RenderPassInfo {
        std::vector<VkAttachmentDescription> Attachments;
        std::vector<RenderSubpass>           Subpasses;

        RenderPassInfo() = default;
    };


    class FrameBuffer;

    class RenderPass {
    public:
        RenderPass()                             = default;
        ~RenderPass()                            = default;
        RenderPass(const RenderPass&)            = delete;
        RenderPass& operator=(const RenderPass&) = delete;
        RenderPass(RenderPass&&)                 = delete;
        RenderPass& operator=(RenderPass&&)      = delete;

        [[nodiscard]] bool Init(LogicalDevice& device, RenderPassInfo info = {});
        void               Destroy(LogicalDevice& device);

        void Begin(CommandBuffer& commandBuffer, FrameBuffer& frameBuffer, VkExtent2D extent,
            const std::vector<VkClearValue>& clearValues);
        void End(CommandBuffer& buffer);

        [[nodiscard]] VkRenderPass GetHandle() const {
            return m_RenderPass;
        }

        VkRenderPass m_RenderPass = VK_NULL_HANDLE;
    };
} // namespace Astrelis::Vulkan
