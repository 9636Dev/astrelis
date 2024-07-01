#include "VulkanRenderSystem.hpp"

#include <vulkan/vulkan.h>

namespace Nebula
{
    void VulkanRenderSystem::StartGraphicsRenderPass()
    {
        VkRenderPassBeginInfo rpBeginInfo {};
        rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBeginInfo.renderPass = m_Context->m_RenderPass.GetHandle();
        // The frame buffer will be the graphics frame buffer
        rpBeginInfo.framebuffer = m_Context->m_SwapChainFrames[m_Context->m_CurrentFrame].RendererFrameBuffer.GetHandle();
        rpBeginInfo.renderArea.offset = m_Context->m_RendererOffset;
        rpBeginInfo.renderArea.extent = m_Context->m_RendererExtent;

        // TODO: Make customizable
        VkClearValue clearValues;
        clearValues.color = {{0.0F, 0.0F, 0.0F, 1.0F}};

        rpBeginInfo.clearValueCount = 1;
        rpBeginInfo.pClearValues = &clearValues;
    }
} // namespace Nebula
