#include "Vulkan2DRendererAPI.hpp"

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Core/Bounds.hpp"

#include "Platform/Vulkan/VK/DescriptorSets.hpp"
#include "Platform/Vulkan/VK/TextureSampler.hpp"
#include "VK/DescriptorSetLayout.hpp"
#include "VK/GraphicsPipeline.hpp"
#include "VK/IndexBuffer.hpp"
#include "VK/TextureImage.hpp"
#include "VK/UniformBuffer.hpp"
#include "VK/VertexBuffer.hpp"
#include "VulkanGraphicsContext.hpp"

#include <utility>

namespace Nebula
{
    Vulkan2DRendererAPI::Vulkan2DRendererAPI(RefPtr<VulkanGraphicsContext> context, Bounds viewport) :
        m_Context(std::move(context)),
        m_Viewport(viewport)
    {
    }

    void Vulkan2DRendererAPI::Init()
    {
        NEBULA_CORE_ASSERT(m_Context->IsInitialized(), "Vulkan Graphics Context not initialized!");
    }

    void Vulkan2DRendererAPI::Shutdown()
    {
        NEBULA_CORE_ASSERT(m_Context->IsInitialized(), "RendererAPI should be destroyed before GraphicsContext!");
    }

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CHECK_RETURN(x)                                                            \
    if (!(x))                                                                      \
    {                                                                              \
        NEBULA_CORE_LOG_ERROR("Failed to create Vulkan2DRendererAPI components!"); \
        return Renderer2DStorage();                                                \
    }

    Renderer2DStorage Vulkan2DRendererAPI::CreateComponents(CreateDetails& details)
    {
        Renderer2DStorage storage;

        RefPtr<Vulkan::GraphicsPipeline> graphicsPipeline = RefPtr<Vulkan::GraphicsPipeline>::Create();
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        descriptorSetLayouts.reserve(details.DescriptorSetLayouts.size());
        for (const auto& layout : details.DescriptorSetLayouts)
        {
            descriptorSetLayouts.push_back(layout.As<Vulkan::DescriptorSetLayout>()->m_Layout);
        }
        CHECK_RETURN(graphicsPipeline->Init(m_Context->m_LogicalDevice, m_Context->m_RenderPass, m_Context->m_SwapChain,
                                            details.VertexInput, descriptorSetLayouts));
        storage.m_GraphicsPipeline = static_cast<RefPtr<GraphicsPipeline>>(graphicsPipeline);
        return storage;
    }

#undef CHECK_RETURN

    void Vulkan2DRendererAPI::DestroyComponents(Renderer2DStorage& storage)
    {
        storage.m_GraphicsPipeline.As<Vulkan::GraphicsPipeline>()->Destroy(m_Context->m_LogicalDevice);
    }

    void Vulkan2DRendererAPI::SetViewport(Viewport& viewport)
    {
        VkViewport vkViewport {};
        vkViewport.x        = viewport.X;
        vkViewport.y        = viewport.Y;
        vkViewport.width    = viewport.Width;
        vkViewport.height   = viewport.Height;
        vkViewport.minDepth = viewport.MinDepth;
        vkViewport.maxDepth = viewport.MaxDepth;

        vkCmdSetViewport(m_Context->GetCurrentFrame().CommandBuffer.GetHandle(), 0, 1, &vkViewport);
    }

    void Vulkan2DRendererAPI::SetScissor(Bounds& scissor)
    {
        VkRect2D vkScissor {};
        vkScissor.offset = {scissor.X, scissor.Y};
        vkScissor.extent = {static_cast<std::uint32_t>(scissor.Width), static_cast<std::uint32_t>(scissor.Height)};

        vkCmdSetScissor(m_Context->GetCurrentFrame().CommandBuffer.GetHandle(), 0, 1, &vkScissor);
    }

    void Vulkan2DRendererAPI::WaitDeviceIdle() { vkDeviceWaitIdle(m_Context->m_LogicalDevice.GetHandle()); }

    void Vulkan2DRendererAPI::DrawInstanced(std::uint32_t vertexCount,
                                            std::uint32_t instanceCount,
                                            std::uint32_t firstVertex,
                                            std::uint32_t firstInstance)
    {
        vkCmdDraw(m_Context->GetCurrentFrame().CommandBuffer.GetHandle(), vertexCount, instanceCount, firstVertex,
                  firstInstance);
    }

    void Vulkan2DRendererAPI::DrawInstancedIndexed(std::uint32_t indexCount,
                                                   std::uint32_t instanceCount,
                                                   std::uint32_t firstIndex,
                                                   std::uint32_t vertexOffset,
                                                   std::uint32_t firstInstance)
    {
        vkCmdDrawIndexed(m_Context->GetCurrentFrame().CommandBuffer.GetHandle(), indexCount, instanceCount, firstIndex,
                         static_cast<std::int32_t>(vertexOffset), firstInstance);
    }

    Bounds Vulkan2DRendererAPI::GetSurfaceSize()
    {
        VkExtent2D extent = m_Context->m_SwapChain.GetExtent();
        return Bounds(0, 0, static_cast<std::int32_t>(extent.width), static_cast<std::int32_t>(extent.height));
    }

    void Vulkan2DRendererAPI::CorrectProjection(glm::mat4& projection) { projection[1][1] *= -1.0F; }

    RefPtr<VertexBuffer> Vulkan2DRendererAPI::CreateVertexBuffer()
    {
        return static_cast<RefPtr<VertexBuffer>>(RefPtr<Vulkan::VertexBuffer>::Create());
    }

    RefPtr<IndexBuffer> Vulkan2DRendererAPI::CreateIndexBuffer()
    {
        return static_cast<RefPtr<IndexBuffer>>(RefPtr<Vulkan::IndexBuffer>::Create());
    }

    RefPtr<DescriptorSetLayout> Vulkan2DRendererAPI::CreateDescriptorSetLayout()
    {
        return static_cast<RefPtr<DescriptorSetLayout>>(RefPtr<Vulkan::DescriptorSetLayout>::Create());
    }

    RefPtr<DescriptorSets> Vulkan2DRendererAPI::CreateDescriptorSets()
    {
        return static_cast<RefPtr<DescriptorSets>>(RefPtr<Vulkan::DescriptorSets>::Create());
    }

    RefPtr<UniformBuffer> Vulkan2DRendererAPI::CreateUniformBuffer()
    {
        return static_cast<RefPtr<UniformBuffer>>(RefPtr<Vulkan::UniformBuffer>::Create());
    }

    RefPtr<TextureImage> Vulkan2DRendererAPI::CreateTextureImage()
    {
        return static_cast<RefPtr<TextureImage>>(RefPtr<Vulkan::TextureImage>::Create());
    }

    RefPtr<TextureSampler> Vulkan2DRendererAPI::CreateTextureSampler()
    {
        return static_cast<RefPtr<TextureSampler>>(RefPtr<Vulkan::TextureSampler>::Create());
    }

    RefPtr<Vulkan2DRendererAPI> Vulkan2DRendererAPI::Create(RefPtr<VulkanGraphicsContext> context, Bounds viewport)
    {
        return RefPtr<Vulkan2DRendererAPI>::Create(context, viewport);
    }
} // namespace Nebula
