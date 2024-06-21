#include "Vulkan2DRendererAPI.hpp"

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Core/Bounds.hpp"

#include "Platform/Vulkan/VK/DescriptorSet.hpp"
#include "Platform/Vulkan/VK/TextureSampler.hpp"
#include "VK/DescriptorSetLayout.hpp"
#include "VK/GraphicsPipeline.hpp"
#include "VK/IndexBuffer.hpp"
#include "VK/UniformBuffer.hpp"
#include "VK/VertexBuffer.hpp"
#include "VK/TextureImage.hpp"
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

        RefPtr<Vulkan::DescriptorSetLayout> descriptorSetLayout = RefPtr<Vulkan::DescriptorSetLayout>::Create();
        CHECK_RETURN(descriptorSetLayout->Init(m_Context->m_LogicalDevice, details.UniformDescriptors, details.SamplerDescriptors));
        storage.m_DescriptorSetLayout = static_cast<RefPtr<DescriptorSetLayout>>(descriptorSetLayout);

        std::vector<VkDescriptorSetLayout> layouts = {descriptorSetLayout->m_Layout};

        RefPtr<Vulkan::GraphicsPipeline> graphicsPipeline = RefPtr<Vulkan::GraphicsPipeline>::Create();
        CHECK_RETURN(graphicsPipeline->Init(m_Context->m_LogicalDevice, m_Context->m_RenderPass, m_Context->m_SwapChain,
                                            details.VertexInput, layouts));
        storage.m_GraphicsPipeline = static_cast<RefPtr<GraphicsPipeline>>(graphicsPipeline);

        RefPtr<Vulkan::VertexBuffer> vertexBuffer = RefPtr<Vulkan::VertexBuffer>::Create();
        CHECK_RETURN(
            vertexBuffer->Init(m_Context->m_PhysicalDevice, m_Context->m_LogicalDevice, details.VertexBufferSize));
        storage.m_VertexBuffer = static_cast<RefPtr<VertexBuffer>>(vertexBuffer);

        RefPtr<Vulkan::IndexBuffer> indexBuffer = RefPtr<Vulkan::IndexBuffer>::Create();
        CHECK_RETURN(indexBuffer->Init(m_Context->m_PhysicalDevice, m_Context->m_LogicalDevice, details.IndicesCount));
        storage.m_IndexBuffer = static_cast<RefPtr<IndexBuffer>>(indexBuffer);

        storage.m_UniformBuffers.reserve(m_Context->m_MaxFramesInFlight);
        storage.m_DescriptorSets.reserve(m_Context->m_MaxFramesInFlight * (details.UniformDescriptors.size() + details.SamplerDescriptors.size()));
        // Calculate total size of uniform buffer
        std::uint32_t uniformBufferSize = 0;
        for (const auto& descriptor : details.UniformDescriptors)
        {
            uniformBufferSize += descriptor.Size;
        }

        for (std::size_t i = 0; i < m_Context->m_MaxFramesInFlight; ++i)
        {
            RefPtr<Vulkan::UniformBuffer> uniformBuffer = RefPtr<Vulkan::UniformBuffer>::Create();
            CHECK_RETURN(
                uniformBuffer->Init(m_Context->m_PhysicalDevice, m_Context->m_LogicalDevice, uniformBufferSize));
            storage.m_UniformBuffers.push_back(static_cast<RefPtr<UniformBuffer>>(uniformBuffer));

            std::uint32_t offset = 0;
            for (auto& descriptor : details.UniformDescriptors)
            {
                RefPtr<Vulkan::DescriptorSet> descriptorSet = RefPtr<Vulkan::DescriptorSet>::Create();
                Vulkan::DescriptorSetInfo descriptorSetInfo {};
                descriptorSetInfo.Buffer = uniformBuffer->GetBuffer();
                descriptorSetInfo.Offset = offset;
                descriptorSetInfo.Size   = descriptor.Size;
                descriptorSetInfo.Binding = descriptor.Binding;
                descriptorSetInfo.Type = Vulkan::DescriptorType::UniformBuffer;
                offset += descriptor.Size;
                CHECK_RETURN(descriptorSet->Init(m_Context->m_LogicalDevice, m_Context->m_DescriptorPool,
                                                descriptorSetLayout->m_Layout, descriptorSetInfo));
                storage.m_DescriptorSets.push_back(static_cast<RefPtr<DescriptorSet>>(descriptorSet));
            }

            for (auto& descriptor : details.SamplerDescriptors)
            {
                RefPtr<Vulkan::DescriptorSet> descriptorSet = RefPtr<Vulkan::DescriptorSet>::Create();
                Vulkan::DescriptorSetInfo descriptorSetInfo {};
                descriptorSetInfo.Sampler = descriptor.Sampler.As<Vulkan::TextureSampler>()->m_Sampler;
                descriptorSetInfo.ImageView = descriptor.Texture.As<Vulkan::TextureImage>()->m_ImageView;
                descriptorSetInfo.Binding = descriptor.Binding;
                descriptorSetInfo.Type = Vulkan::DescriptorType::Sampler;
                CHECK_RETURN(descriptorSet->Init(m_Context->m_LogicalDevice, m_Context->m_DescriptorPool,
                                                descriptorSetLayout->m_Layout, descriptorSetInfo));
                storage.m_DescriptorSets.push_back(static_cast<RefPtr<DescriptorSet>>(descriptorSet));
            }
        }

        // Set the indices map
        std::uint32_t index = 0;
        for (const auto& descriptor : details.UniformDescriptors)
        {
            storage.m_DescriptorSetIndices[descriptor.Name] = index++;
        }
        for (const auto& descriptor : details.SamplerDescriptors)
        {
            storage.m_DescriptorSetIndices[descriptor.Name] = index++;
        }

        return storage;
    }

#undef CHECK_RETURN

    void Vulkan2DRendererAPI::DestroyComponents(Renderer2DStorage& storage)
    {
        for (auto& uniformBuffer : storage.m_UniformBuffers)
        {
            uniformBuffer.As<Vulkan::UniformBuffer>()->Destroy(m_Context->m_LogicalDevice);
        }
        storage.m_IndexBuffer.As<Vulkan::IndexBuffer>()->Destroy(m_Context->m_LogicalDevice);
        storage.m_VertexBuffer.As<Vulkan::VertexBuffer>()->Destroy(m_Context->m_LogicalDevice);

        storage.m_DescriptorSetLayout.As<Vulkan::DescriptorSetLayout>()->Destroy(m_Context->m_LogicalDevice);
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

    void Vulkan2DRendererAPI::DrawInstancedIndexed(
                                                   std::uint32_t indexCount,
                                                   std::uint32_t instanceCount,
                                                   std::uint32_t firstIndex,
                                                   std::uint32_t vertexOffset,
                                                   std::uint32_t firstInstance)
    {
        vkCmdDrawIndexed(m_Context->GetCurrentFrame().CommandBuffer.GetHandle(), indexCount, instanceCount,
                         firstIndex, static_cast<std::int32_t>(vertexOffset), firstInstance);
    }

    Bounds Vulkan2DRendererAPI::GetSurfaceSize()
    {
        VkExtent2D extent = m_Context->m_SwapChain.GetExtent();
        return Bounds(0, 0, static_cast<std::int32_t>(extent.width), static_cast<std::int32_t>(extent.height));
    }

    void Vulkan2DRendererAPI::CorrectProjection(glm::mat4& projection)
    {
        projection[1][1] *= -1.0F;
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
