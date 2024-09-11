#include "Vulkan2DRendererAPI.hpp"
#include "Astrelis/Core/Base.hpp"

#include "Platform/Vulkan/VK/DescriptorSets.hpp"
#include "Platform/Vulkan/VK/TextureSampler.hpp"
#include "VK/DescriptorSetLayout.hpp"
#include "VK/GraphicsPipeline.hpp"
#include "VK/IndexBuffer.hpp"
#include "VK/TextureImage.hpp"
#include "VK/UniformBuffer.hpp"
#include "VK/VertexBuffer.hpp"
#include "VulkanGraphicsContext.hpp"

namespace Astrelis
{
    Vulkan2DRendererAPI::Vulkan2DRendererAPI(RefPtr<VulkanGraphicsContext> context) :
        m_Context(std::move(context))
    {
    }

    void Vulkan2DRendererAPI::Init()
    {
        ASTRELIS_CORE_ASSERT(m_Context->IsInitialized(), "Vulkan Graphics Context not initialized!");
    }

    void Vulkan2DRendererAPI::Shutdown()
    {
        ASTRELIS_CORE_ASSERT(m_Context->IsInitialized(), "RendererAPI should be destroyed before GraphicsContext!");
    }

    void Vulkan2DRendererAPI::SetViewport(Rect3Df& viewport)
    {
        VkViewport vkViewport {};
        vkViewport.x        = viewport.X();
        vkViewport.y        = viewport.Y();
        vkViewport.width    = viewport.Width();
        vkViewport.height   = viewport.Height();
        vkViewport.minDepth = viewport.Z();
        vkViewport.maxDepth = viewport.Depth();

        vkCmdSetViewport(m_Context->GetCurrentFrame().CommandBuffer.GetHandle(), 0, 1, &vkViewport);
    }

    void Vulkan2DRendererAPI::SetScissor(Rect2Di& scissor)
    {
        VkRect2D vkScissor {};
        vkScissor.offset = {scissor.X(), scissor.Y()};
        vkScissor.extent = {static_cast<std::uint32_t>(scissor.Width()), static_cast<std::uint32_t>(scissor.Height())};

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

    Rect2Di Vulkan2DRendererAPI::GetSurfaceSize()
    {
        VkExtent2D extent = m_Context->m_SwapChain.GetExtent();
        return Rect2Di(0, 0, static_cast<std::int32_t>(extent.width), static_cast<std::int32_t>(extent.height));
    }

    void Vulkan2DRendererAPI::CorrectProjection(Mat4f& projection) { projection[1][1] *= -1.0F; }

    RefPtr<GraphicsPipeline> Vulkan2DRendererAPI::CreateGraphicsPipeline()
    {
        return static_cast<RefPtr<GraphicsPipeline>>(RefPtr<Vulkan::GraphicsPipeline>::Create());
    }

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

    RefPtr<Vulkan2DRendererAPI> Vulkan2DRendererAPI::Create(RefPtr<VulkanGraphicsContext> context)
    {
        return RefPtr<Vulkan2DRendererAPI>::Create(context);
    }
} // namespace Astrelis
