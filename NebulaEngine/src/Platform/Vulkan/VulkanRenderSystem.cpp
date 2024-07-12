#include "VulkanRenderSystem.hpp"
#include "NebulaEngine/Renderer/DescriptorSetLayout.hpp"
#include "NebulaEngine/Renderer/GraphicsPipeline.hpp"
#include "NebulaEngine/Renderer/TextureImage.hpp"
#include "Platform/Vulkan/VK/DescriptorSetLayout.hpp"
#include "Platform/Vulkan/VK/TextureSampler.hpp"
#include "Platform/Vulkan/VK/Utils.hpp"

#include <cstddef>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace Nebula
{
    struct BlitVertex
    {
        glm::vec2 PosAndTex;
    };

    bool VulkanRenderSystem::Init()
    {
        std::array<BlitVertex, 4> vertices = {
            BlitVertex {{-1.0F, -1.0F}},
            BlitVertex {{1.0F, -1.0F}},
            BlitVertex {{1.0F, 1.0F}},
            BlitVertex {{-1.0F, 1.0F}},
        };

        std::array<std::uint32_t, 6> indices = {
            0, 1, 2, 2, 3, 0,
        };

        if (!m_VertexBuffer.Init(m_Context->m_LogicalDevice, m_Context->m_PhysicalDevice,
                                 vertices.size() * sizeof(BlitVertex)) ||
            !m_VertexBuffer.SetData(m_Context->m_LogicalDevice, m_Context->m_PhysicalDevice, m_Context->m_CommandPool,
                                    vertices.data(), vertices.size() * sizeof(BlitVertex)) ||
            !m_IndexBuffer.Init(m_Context->m_LogicalDevice, m_Context->m_PhysicalDevice, indices.size()) ||
            !m_IndexBuffer.SetData(m_Context->m_LogicalDevice, m_Context->m_PhysicalDevice, m_Context->m_CommandPool,
                                   indices.data(), indices.size()))
        {
            return false;
        }

        PipelineShaders shaders;
        shaders.Vertex   = "resources/shaders/Blit_vert.spv";
        shaders.Fragment = "resources/shaders/Blit_frag.spv";

        std::vector<BufferBinding> inputs(1);
        inputs[0].Binding  = 0;
        inputs[0].Stride   = sizeof(BlitVertex);
        inputs[0].Elements = {
            {VertexInput::VertexType::Float, offsetof(BlitVertex, PosAndTex), 2, 0},
        };

        m_GraphicsTextureSampler = RefPtr<Vulkan::TextureSampler>::Create();
        m_GraphicsTextureSampler->Init(m_Context->m_LogicalDevice, m_Context->m_PhysicalDevice);

        std::vector<BindingDescriptor> graphicsBindings = {
            BindingDescriptor::TextureSampler("GraphicsSampler", 0,
                                              static_cast<RefPtr<TextureImage>>(m_Context->m_GraphicsTextureImage),
                                              static_cast<RefPtr<TextureSampler>>(m_GraphicsTextureSampler)),
        };

        auto ctx = static_cast<RefPtr<GraphicsContext>>(m_Context);
        if (!m_DescriptorSetLayout.Init(ctx, graphicsBindings))
        {
            return false;
        }

        if (!m_DescriptorSets.Init(m_Context->m_LogicalDevice, m_Context->m_DescriptorPool, m_DescriptorSetLayout,
                                   graphicsBindings))
        {
            return false;
        }

        std::vector<Vulkan::DescriptorSetLayout> layouts = {m_DescriptorSetLayout};
        return m_GraphicsPipeline.Init(m_Context->m_LogicalDevice, m_Context->m_SwapChain.GetExtent(),
                                       m_Context->m_RenderPass, shaders, inputs, layouts);
    }

    void VulkanRenderSystem::Shutdown()
    {
        vkDeviceWaitIdle(m_Context->m_LogicalDevice.GetHandle());
        auto ctx = static_cast<RefPtr<GraphicsContext>>(m_Context);
        m_GraphicsPipeline.Destroy(ctx);
        m_DescriptorSets.Destroy(ctx);
        m_DescriptorSetLayout.Destroy(ctx);
        m_GraphicsTextureSampler->Destroy(ctx);
        m_VertexBuffer.Destroy(ctx);
        m_IndexBuffer.Destroy(ctx);
    }

    void VulkanRenderSystem::StartGraphicsRenderPass()
    {
        auto& frame = m_Context->GetCurrentFrame();
        m_Context->m_GraphicsRenderPass.Begin(frame.CommandBuffer, m_Context->m_GraphicsFrameBuffer,
                                              m_Context->m_GraphicsExtent);
    }

    void VulkanRenderSystem::EndGraphicsRenderPass()
    {
        m_Context->m_GraphicsRenderPass.End(m_Context->GetCurrentFrame().CommandBuffer);
    }

    void VulkanRenderSystem::BlitSwapchain()
    {
        auto& frame          = m_Context->GetCurrentFrame();
        auto& swapchainFrame = m_Context->m_SwapChainFrames[m_Context->m_ImageIndex];
        m_Context->m_RenderPass.Begin(frame.CommandBuffer, swapchainFrame.FrameBuffer,
                                      m_Context->m_SwapChain.GetExtent());
        // We just render normally, because we didn't use the blit extension

        vkCmdBindPipeline(frame.CommandBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                          m_GraphicsPipeline.m_Pipeline);

        m_VertexBuffer.Bind(frame.CommandBuffer, 0);
        m_IndexBuffer.Bind(frame.CommandBuffer);
        m_DescriptorSets.Bind(frame.CommandBuffer, m_GraphicsPipeline);

        vkCmdDrawIndexed(frame.CommandBuffer.GetHandle(), 6, 1, 0, 0, 0);
    }

    void VulkanRenderSystem::EndFrame() { m_Context->m_RenderPass.End(m_Context->GetCurrentFrame().CommandBuffer); }

    InMemoryImage VulkanRenderSystem::CaptureFrame()
    {
        // First create an image with the same extent as the swapchain and VK_IMAGE_USAGE_TRANSFER_DST_BIT
        VkImage image {};
        VkDeviceMemory imageMemory {};
        Vulkan::CreateImage(
            m_Context->m_PhysicalDevice.GetHandle(), m_Context->m_LogicalDevice.GetHandle(),
            m_Context->m_SwapChain.GetExtent().width, m_Context->m_SwapChain.GetExtent().height,
            m_Context->m_SwapChain.GetImageFormat(), VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, image, imageMemory);

        // Transition the image layout to VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        Vulkan::TransitionImageLayout(
            m_Context->m_LogicalDevice.GetHandle(), m_Context->m_LogicalDevice.GetGraphicsQueue(),
            m_Context->m_CommandPool.GetHandle(), image, m_Context->m_SwapChain.GetImageFormat(),
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        // Transition swapchain to VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
        Vulkan::TransitionImageLayout(
            m_Context->m_LogicalDevice.GetHandle(), m_Context->m_LogicalDevice.GetGraphicsQueue(),
            m_Context->m_CommandPool.GetHandle(), m_Context->m_SwapChain.GetImages()[m_Context->m_ImageIndex],
            m_Context->m_SwapChain.GetImageFormat(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);


        // Copy the swapchain image to the new image
        VkImageCopy copyRegion{};
        copyRegion.srcSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        copyRegion.srcSubresource.layerCount     = 1;
        copyRegion.dstSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        copyRegion.dstSubresource.layerCount     = 1;
        copyRegion.extent.width                  = m_Context->m_SwapChain.GetExtent().width;
        copyRegion.extent.height                 = m_Context->m_SwapChain.GetExtent().height;
        copyRegion.extent.depth                  = 1;
        auto *cmdBuffer = Vulkan::BeginSingleTimeCommands(m_Context->m_LogicalDevice.GetHandle(), m_Context->m_CommandPool.GetHandle());
        vkCmdCopyImage(cmdBuffer,
                       m_Context->m_SwapChain.GetImages()[m_Context->m_ImageIndex], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, image,
                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
        Vulkan::EndSingleTimeCommands(m_Context->m_LogicalDevice.GetHandle(), m_Context->m_LogicalDevice.GetGraphicsQueue(),
                                      m_Context->m_CommandPool.GetHandle(), cmdBuffer);

        // Transition the image layout to VK_IMAGE_LAYOUT_GENERAL
        Vulkan::TransitionImageLayout(
            m_Context->m_LogicalDevice.GetHandle(), m_Context->m_LogicalDevice.GetGraphicsQueue(),
            m_Context->m_CommandPool.GetHandle(), image, m_Context->m_SwapChain.GetImageFormat(),
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
        // Map the image memory and copy the data to an InMemoryImage
        void* data = nullptr;
        vkMapMemory(m_Context->m_LogicalDevice.GetHandle(), imageMemory, 0,
                    m_Context->m_SwapChain.GetExtent().width * m_Context->m_SwapChain.GetExtent().height * 4, 0, &data);

        std::vector<std::byte> imageData(static_cast<std::size_t>(m_Context->m_SwapChain.GetExtent().width * m_Context->m_SwapChain.GetExtent().height * 4));
        std::memcpy(imageData.data(), data, imageData.size());
        // Cleanup
        vkUnmapMemory(m_Context->m_LogicalDevice.GetHandle(), imageMemory);
        vkDestroyImage(m_Context->m_LogicalDevice.GetHandle(), image, nullptr);
        vkFreeMemory(m_Context->m_LogicalDevice.GetHandle(), imageMemory, nullptr);

        Vulkan::TransitionImageLayout(
            m_Context->m_LogicalDevice.GetHandle(), m_Context->m_LogicalDevice.GetGraphicsQueue(),
            m_Context->m_CommandPool.GetHandle(), m_Context->m_SwapChain.GetImages()[m_Context->m_ImageIndex],
            m_Context->m_SwapChain.GetImageFormat(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

        return InMemoryImage(m_Context->m_SwapChain.GetExtent().width, m_Context->m_SwapChain.GetExtent().height, 4, imageData);
    }
} // namespace Nebula
