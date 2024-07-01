#include "RenderPass.hpp"

#include "NebulaEngine/Core/Log.hpp"

#include "CommandBuffer.hpp"
#include "FrameBuffer.hpp"
#include <vulkan/vulkan_core.h>

namespace Nebula::Vulkan
{
    bool RenderPass::Init(LogicalDevice& device, RenderPassInfo info)
    {
        VkRenderPassCreateInfo renderPassInfo {};
        renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<std::uint32_t>(info.Attachments.size());
        renderPassInfo.pAttachments    = info.Attachments.data();
        std::vector<VkSubpassDescription> subpasses(info.Subpasses.size());
        for (size_t i = 0; i < info.Subpasses.size(); i++)
        {
            subpasses[i].pipelineBindPoint       = info.Subpasses[i].PipelineBindPoint;
            subpasses[i].colorAttachmentCount    = static_cast<uint32_t>(info.Subpasses[i].Attachments.size());
            subpasses[i].pColorAttachments       = info.Subpasses[i].Attachments.data();
            subpasses[i].inputAttachmentCount    = 0;
            subpasses[i].pInputAttachments       = nullptr;
            subpasses[i].pResolveAttachments     = nullptr;
            subpasses[i].pDepthStencilAttachment = nullptr;
            subpasses[i].preserveAttachmentCount = 0;
            subpasses[i].pPreserveAttachments    = nullptr;
        }
        renderPassInfo.subpassCount    = static_cast<uint32_t>(subpasses.size());
        renderPassInfo.pSubpasses      = subpasses.data();

        VkSubpassDependency dependency {};
        dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass    = 0;
        dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies   = &dependency;


        if (vkCreateRenderPass(device.GetHandle(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create render pass!");
            return false;
        }

        return true;
    }

    void RenderPass::Destroy(LogicalDevice& device) { vkDestroyRenderPass(device.GetHandle(), m_RenderPass, nullptr); }

    void RenderPass::Begin(CommandBuffer& commandBuffer, FrameBuffer& frameBuffer, VkExtent2D extent)
    {
        VkRenderPassBeginInfo renderPassInfo {};
        renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass        = m_RenderPass;
        renderPassInfo.framebuffer       = frameBuffer.GetHandle();
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = extent;

        VkClearValue clearColor        = {{{0.0F, 0.0F, 0.0F, 1.0F}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues    = &clearColor;

        VkSubpassBeginInfo subpassBeginInfo {};
        subpassBeginInfo.sType = VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO;
        subpassBeginInfo.contents = VK_SUBPASS_CONTENTS_INLINE;

        vkCmdBeginRenderPass2(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo *pRenderPassBegin, const VkSubpassBeginInfo *pSubpassBeginInfo)
    }

    void RenderPass::End(CommandBuffer& commandBuffer) {
        vkCmdEndRenderPass(commandBuffer.GetHandle());
    }

} // namespace Nebula::Vulkan
