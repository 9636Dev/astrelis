#include "VulkanRenderSystem.hpp"
#include "NebulaEngine/Renderer/DescriptorSetLayout.hpp"
#include "NebulaEngine/Renderer/GraphicsPipeline.hpp"
#include "NebulaEngine/Renderer/TextureImage.hpp"
#include "Platform/Vulkan/VK/DescriptorSetLayout.hpp"

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

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
                                   vertices.data(), vertices.size()) ||
            !m_IndexBuffer.Init(m_Context->m_LogicalDevice, m_Context->m_PhysicalDevice, indices.size()) ||
            !m_IndexBuffer.SetData(m_Context->m_LogicalDevice, m_Context->m_PhysicalDevice, m_Context->m_CommandPool,
                                  indices.data(), indices.size()))
        {
            return false;
        }

        auto& swapChainFrame = m_Context->m_SwapChainFrames[m_Context->m_ImageIndex];
        PipelineShaders shaders;
        shaders.Vertex   = "resources/shaders/Blit_vert.spv";
        shaders.Fragment = "resources/shaders/Blit_frag.spv";

        std::vector<BufferBinding> bindings(1);
        bindings[0].Binding = 0;
        bindings[0].Stride  = sizeof(BlitVertex);
        bindings[0].Elements = {
            {VertexInput::VertexType::Float, offsetof(BlitVertex, PosAndTex), 2, 0},
        };

        std::vector<RefPtr<DescriptorSetLayout>> layouts;
        layouts.push_back(static_cast<RefPtr<DescriptorSetLayout>>(RefPtr<Vulkan::DescriptorSetLayout>::Create()));
        std::vector<BindingDescriptor> descriptors;

        m_GraphicsTextureSampler = RefPtr<Vulkan::TextureSampler>::Create();
        m_GraphicsTextureSampler->Init(m_Context->m_LogicalDevice, m_Context->m_PhysicalDevice);
        auto graphicsSampler = static_cast<RefPtr<TextureSampler>>(m_GraphicsTextureSampler);
        descriptors.push_back(BindingDescriptor::TextureSampler("GraphicsSampler", 0, static_cast<RefPtr<TextureImage>>(swapChainFrame.GraphicsTextureImage), graphicsSampler));

        m_UITextureSampler = RefPtr<Vulkan::TextureSampler>::Create();
        m_UITextureSampler->Init(m_Context->m_LogicalDevice, m_Context->m_PhysicalDevice);
        auto uiSampler = static_cast<RefPtr<TextureSampler>>(m_UITextureSampler);
        descriptors.push_back(BindingDescriptor::TextureSampler("UISampler", 1, static_cast<RefPtr<TextureImage>>(swapChainFrame.UITextureImage), uiSampler));

        auto ctx = static_cast<RefPtr<GraphicsContext>>(m_Context);
        if (!layouts[0]->Init(ctx, descriptors))
        {
            return false;
        }

        return m_GraphicsPipeline.Init(ctx, shaders, bindings, layouts);
    }

    void VulkanRenderSystem::Shutdown()
    {
        /*
        m_GraphicsPipeline.Destroy();
        m_VertexBuffer.Destory();
        m_IndexBuffer.Destroy();
        */
    }

    void VulkanRenderSystem::StartGraphicsRenderPass()
    {
        auto& frame          = m_Context->GetCurrentFrame();
        auto& swapchainFrame = m_Context->m_SwapChainFrames[m_Context->m_ImageIndex];
        m_Context->m_GraphicsRenderPass.Begin(frame.CommandBuffer, swapchainFrame.GraphicsFrameBuffer,
                                              m_Context->m_GraphicsExtent);
    }

    void VulkanRenderSystem::EndGraphicsRenderPass()
    {
        m_Context->m_GraphicsRenderPass.End(m_Context->GetCurrentFrame().CommandBuffer);
    }

    void VulkanRenderSystem::StartOverlayRenderPass()
    {
        auto& frame          = m_Context->GetCurrentFrame();
        auto& swapchainFrame = m_Context->m_SwapChainFrames[m_Context->m_ImageIndex];
        m_Context->m_UIRenderPass.Begin(frame.CommandBuffer, swapchainFrame.UIFrameBuffer, m_Context->m_GraphicsExtent);
    }

    void VulkanRenderSystem::EndOverlayRenderPass()
    {
        m_Context->m_UIRenderPass.End(m_Context->GetCurrentFrame().CommandBuffer);
    }

    void VulkanRenderSystem::BlitSwapchain()
    {
        auto& frame          = m_Context->GetCurrentFrame();
        auto& swapchainFrame = m_Context->m_SwapChainFrames[m_Context->m_ImageIndex];

        // We just render normally, because we didn't use the blit extension
        m_Context->m_RenderPass.Begin(frame.CommandBuffer, swapchainFrame.FrameBuffer,
                                      m_Context->m_SwapChain.GetExtent());

        vkCmdBindPipeline(frame.CommandBuffer.GetHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                          m_GraphicsPipeline.m_Pipeline);

        m_VertexBuffer.Bind(frame.CommandBuffer, 0);
        m_IndexBuffer.Bind(frame.CommandBuffer);

        // Samplers setup using DescriptorSets

        vkCmdDrawIndexed(frame.CommandBuffer.GetHandle(), 6, 1, 0, 0, 0);

        m_Context->m_RenderPass.End(frame.CommandBuffer);
    }

} // namespace Nebula
