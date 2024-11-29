#include "RenderPass.hpp"

#include "Astrelis/Core/Log.hpp"

#include "CommandBuffer.hpp"
#include "FrameBuffer.hpp"

namespace Astrelis::Vulkan {
    bool RenderPass::Init(LogicalDevice& device, RenderPassInfo info) {
        ASTRELIS_CORE_ASSERT(
            info.Attachments.size() > 0, "RenderPass must have at least one attachment!");
        VkRenderPassCreateInfo renderPassInfo {};
        renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<std::uint32_t>(info.Attachments.size());
        renderPassInfo.pAttachments    = info.Attachments.data();
        std::vector<VkSubpassDescription> subpasses(info.Subpasses.size());
        for (size_t i = 0; i < info.Subpasses.size(); i++) {
            ASTRELIS_CORE_ASSERT(info.Subpasses[i].ColorAttachments.size() > 0,
                "Subpass must have at least one color attachment!");
            ASTRELIS_CORE_ASSERT(info.Subpasses[i].ColorAttachments.size()
                        == info.Subpasses[i].ResolveAttachments.size()
                    || info.Subpasses[i].ResolveAttachments.empty(),
                "Color and resolve attachments must have the same size!");
            subpasses[i].pipelineBindPoint = info.Subpasses[i].PipelineBindPoint;
            subpasses[i].colorAttachmentCount =
                static_cast<uint32_t>(info.Subpasses[i].ColorAttachments.size());
            subpasses[i].pColorAttachments   = info.Subpasses[i].ColorAttachments.data();
            subpasses[i].pResolveAttachments = info.Subpasses[i].ResolveAttachments.data();

            if (info.Subpasses[i].DepthAttachment.attachment != VK_ATTACHMENT_UNUSED) {
                subpasses[i].pDepthStencilAttachment = &info.Subpasses[i].DepthAttachment;
            }
        }
        renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
        renderPassInfo.pSubpasses   = subpasses.data();

        std::array<VkSubpassDependency, 1> dependencies {};
        dependencies[0].srcSubpass   = VK_SUBPASS_EXTERNAL;
        dependencies[0].dstSubpass   = 0;
        dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
            | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependencies[0].srcAccessMask = 0;
        dependencies[0].dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
            | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependencies[0].dstAccessMask =
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
        renderPassInfo.pDependencies   = dependencies.data();


        if (vkCreateRenderPass(device.GetHandle(), &renderPassInfo, nullptr, &m_RenderPass)
            != VK_SUCCESS) {
            ASTRELIS_CORE_LOG_ERROR("Failed to create render pass!");
            return false;
        }

        return true;
    }

    void RenderPass::Destroy(LogicalDevice& device) {
        vkDestroyRenderPass(device.GetHandle(), m_RenderPass, nullptr);
    }

    void RenderPass::Begin(CommandBuffer& commandBuffer, FrameBuffer& frameBuffer,
        VkExtent2D extent, const std::vector<VkClearValue>& clearValues) {
        ASTRELIS_CORE_ASSERT(frameBuffer.GetHandle() != VK_NULL_HANDLE,
            "FrameBuffer must be created before calling Begin on RenderPass!");
        VkRenderPassBeginInfo renderPassInfo {};
        renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass        = m_RenderPass;
        renderPassInfo.framebuffer       = frameBuffer.GetHandle();
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = extent;
        renderPassInfo.clearValueCount   = static_cast<std::uint32_t>(clearValues.size());
        renderPassInfo.pClearValues      = clearValues.data();

        VkSubpassBeginInfo subpassBeginInfo {};
        subpassBeginInfo.sType    = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO;
        subpassBeginInfo.contents = VK_SUBPASS_CONTENTS_INLINE;

        vkCmdBeginRenderPass(
            commandBuffer.GetHandle(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void RenderPass::End(CommandBuffer& commandBuffer) {
        vkCmdEndRenderPass(commandBuffer.GetHandle());
    }

} // namespace Astrelis::Vulkan
