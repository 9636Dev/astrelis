#include "VulkanRenderSystem.hpp"
#include "NebulaEngine/Core/Math.hpp"
#include "NebulaEngine/Renderer/DescriptorSetLayout.hpp"
#include "NebulaEngine/Renderer/GraphicsPipeline.hpp"
#include "NebulaEngine/Renderer/TextureImage.hpp"
#include "Platform/Vulkan/VK/DescriptorSetLayout.hpp"
#include "Platform/Vulkan/VK/TextureSampler.hpp"
#include "Platform/Vulkan/VK/Utils.hpp"

#include <cstddef>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

namespace Nebula
{
    struct BlitVertex
    {
        Vec2f PosAndTex;
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

        if (!m_BlitSwapchain)
        {
            return;
        }

        // We just render normally, because we didn't use the blit extension

        vkCmdBindPipeline(frame.CommandBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                          m_GraphicsPipeline.m_Pipeline);

        m_VertexBuffer.Bind(frame.CommandBuffer, 0);
        m_IndexBuffer.Bind(frame.CommandBuffer);
        m_DescriptorSets.Bind(frame.CommandBuffer, m_GraphicsPipeline);

        // Dynamic state
        VkViewport viewport {};
        viewport.x        = 0.0F;
        viewport.y        = 0.0F;
        viewport.width    = static_cast<float>(m_Context->m_SwapChain.GetExtent().width);
        viewport.height   = static_cast<float>(m_Context->m_SwapChain.GetExtent().height);
        viewport.minDepth = 0.0F;
        viewport.maxDepth = 0.0F; // It only needs to be 0.0F

        vkCmdSetViewport(frame.CommandBuffer.GetHandle(), 0, 1, &viewport);

        VkRect2D scissor {};
        scissor.extent = m_Context->m_SwapChain.GetExtent();

        vkCmdSetScissor(frame.CommandBuffer.GetHandle(), 0, 1, &scissor);

        vkCmdDrawIndexed(frame.CommandBuffer.GetHandle(), 6, 1, 0, 0, 0);
    }

    void VulkanRenderSystem::EndFrame() { m_Context->m_RenderPass.End(m_Context->GetCurrentFrame().CommandBuffer); }

    std::future<InMemoryImage> VulkanRenderSystem::CaptureFrame()
    {
        m_Context->m_CaptureNextFrame = true;
        return m_Context->m_CapturePromise.get_future();
    }
} // namespace Nebula
