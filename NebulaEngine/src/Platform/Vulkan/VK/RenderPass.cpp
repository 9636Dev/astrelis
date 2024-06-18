#include "RenderPass.hpp"

#include "NebulaEngine/Core/Log.hpp"

#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

#include "SwapChainFrameBuffers.hpp"
#include "CommandBuffer.hpp"

namespace Nebula::Vulkan
{
    bool RenderPass::Init(LogicalDevice& device, SwapChain& swapChain)
    {
        VkAttachmentDescription colorAttachment {};
        colorAttachment.format         = swapChain.GetImageFormat();
        colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass {};
        subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments    = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo {};
        renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments    = &colorAttachment;
        renderPassInfo.subpassCount    = 1;
        renderPassInfo.pSubpasses      = &subpass;

        VkSubpassDependency dependency {};
        dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass    = 0;
        dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies    = &dependency;


        if (vkCreateRenderPass(device.GetHandle(), &renderPassInfo,
                               nullptr, &m_RenderPass) != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to create render pass!");
            return false;
        }

        return true;
    }

    void RenderPass::Destroy(LogicalDevice& device)
    {
        vkDestroyRenderPass(device.GetHandle(), m_RenderPass, nullptr);
    }

    void RenderPass::Begin(RefPtr<GraphicsContext>& context, RefPtr<Nebula::CommandBuffer>& commandBuffer, RefPtr<Nebula::SwapChainFrameBuffers>& frameBuffers, std::uint32_t frameIndex)
    {
        VkRenderPassBeginInfo renderPassInfo {};
        renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass        = m_RenderPass;
        renderPassInfo.framebuffer       = frameBuffers.As<SwapChainFrameBuffers>()->m_SwapChainFrameBuffers[frameIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = context.As<VulkanGraphicsContext>()->m_SwapChain.GetExtent();

        VkClearValue clearColor = {{{0.0F, 0.0F, 0.0F, 1.0F}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues    = &clearColor;

        vkCmdBeginRenderPass(commandBuffer.As<Vulkan::CommandBuffer>()->m_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void RenderPass::End([[maybe_unused]] RefPtr<GraphicsContext>& context, RefPtr<Nebula::CommandBuffer>& commandBuffer)
    {
        vkCmdEndRenderPass(commandBuffer.As<Vulkan::CommandBuffer>()->m_CommandBuffer);
    }

} // namespace Nebula::Vulkan
