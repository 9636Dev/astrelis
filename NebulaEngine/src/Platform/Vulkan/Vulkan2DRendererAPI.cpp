#include "Vulkan2DRendererAPI.hpp"

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Core/Bounds.hpp"

#include "VK/CommandBuffer.hpp"
#include "VK/CommandPool.hpp"
#include "VK/Fence.hpp"
#include "VK/GraphicsPipeline.hpp"
#include "VK/ImageViews.hpp"
#include "VK/IndexBuffer.hpp"
#include "VK/RenderPass.hpp"
#include "VK/Semaphore.hpp"
#include "VK/SwapChainFrameBuffers.hpp"
#include "VK/VertexBuffer.hpp"
#include "VulkanGraphicsContext.hpp"

#include <array>
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

        RefPtr<Vulkan::CommandPool> commandPool = RefPtr<Vulkan::CommandPool>::Create();
        CHECK_RETURN(commandPool->Init(m_Context->m_LogicalDevice));
        storage.m_CommandPool = static_cast<RefPtr<CommandPool>>(commandPool);

        std::vector<RefPtr<CommandBuffer>> commandBuffers;
        commandBuffers.reserve(details.MaxFramesInFlight);
        for (std::size_t i = 0; i < details.MaxFramesInFlight; ++i)
        {
            RefPtr<Vulkan::CommandBuffer> commandBuffer = RefPtr<Vulkan::CommandBuffer>::Create();
            CHECK_RETURN(commandBuffer->Init(m_Context->m_LogicalDevice, *commandPool));
            commandBuffers.push_back(static_cast<RefPtr<CommandBuffer>>(commandBuffer));
        }
        storage.m_CommandBuffers = std::move(commandBuffers);

        RefPtr<Vulkan::ImageViews> imageViews = RefPtr<Vulkan::ImageViews>::Create();
        CHECK_RETURN(imageViews->Init(m_Context->m_LogicalDevice, m_Context->m_SwapChain));
        storage.m_ImageViews = static_cast<RefPtr<ImageViews>>(imageViews);

        RefPtr<Vulkan::RenderPass> renderPass = RefPtr<Vulkan::RenderPass>::Create();
        CHECK_RETURN(renderPass->Init(m_Context->m_LogicalDevice, m_Context->m_SwapChain));
        storage.m_RenderPass = static_cast<RefPtr<RenderPass>>(renderPass);

        RefPtr<Vulkan::SwapChainFrameBuffers> frameBuffers = RefPtr<Vulkan::SwapChainFrameBuffers>::Create();
        CHECK_RETURN(frameBuffers->Init(m_Context->m_LogicalDevice, *renderPass, *imageViews, m_Context->m_SwapChain));
        storage.m_FrameBuffers = static_cast<RefPtr<SwapChainFrameBuffers>>(frameBuffers);

        RefPtr<Vulkan::GraphicsPipeline> graphicsPipeline = RefPtr<Vulkan::GraphicsPipeline>::Create();
        CHECK_RETURN(graphicsPipeline->Init(m_Context->m_LogicalDevice, *renderPass, m_Context->m_SwapChain, details.VertexInput));
        storage.m_GraphicsPipeline = static_cast<RefPtr<GraphicsPipeline>>(graphicsPipeline);

        RefPtr<Vulkan::VertexBuffer> vertexBuffer = RefPtr<Vulkan::VertexBuffer>::Create();
        CHECK_RETURN(
            vertexBuffer->Init(m_Context->m_PhysicalDevice, m_Context->m_LogicalDevice, details.VertexBufferSize));
        storage.m_VertexBuffer = static_cast<RefPtr<VertexBuffer>>(vertexBuffer);

        RefPtr<Vulkan::IndexBuffer> indexBuffer = RefPtr<Vulkan::IndexBuffer>::Create();
        CHECK_RETURN(indexBuffer->Init(m_Context->m_PhysicalDevice, m_Context->m_LogicalDevice, details.IndicesCount));
        storage.m_IndexBuffer = static_cast<RefPtr<IndexBuffer>>(indexBuffer);

        std::vector<RefPtr<Semaphore>> imageAvailableSemaphores;
        std::vector<RefPtr<Semaphore>> renderFinishedSemaphores;
        std::vector<RefPtr<Fence>> inFlightFences;
        imageAvailableSemaphores.reserve(details.MaxFramesInFlight);
        renderFinishedSemaphores.reserve(details.MaxFramesInFlight);
        inFlightFences.reserve(details.MaxFramesInFlight);

        for (std::size_t i = 0; i < details.MaxFramesInFlight; ++i)
        {
            RefPtr<Vulkan::Semaphore> imageAvailableSemaphore = RefPtr<Vulkan::Semaphore>::Create();
            CHECK_RETURN(imageAvailableSemaphore->Init(m_Context->m_LogicalDevice));
            imageAvailableSemaphores.push_back(static_cast<RefPtr<Semaphore>>(imageAvailableSemaphore));

            RefPtr<Vulkan::Semaphore> renderFinishedSemaphore = RefPtr<Vulkan::Semaphore>::Create();
            CHECK_RETURN(renderFinishedSemaphore->Init(m_Context->m_LogicalDevice));
            renderFinishedSemaphores.push_back(static_cast<RefPtr<Semaphore>>(renderFinishedSemaphore));

            RefPtr<Vulkan::Fence> inFlightFence = RefPtr<Vulkan::Fence>::Create();
            CHECK_RETURN(inFlightFence->Init(m_Context->m_LogicalDevice));
            inFlightFences.push_back(static_cast<RefPtr<Fence>>(inFlightFence));
        }

        storage.m_ImageAvailableSemaphores = std::move(imageAvailableSemaphores);
        storage.m_RenderFinishedSemaphores = std::move(renderFinishedSemaphores);
        storage.m_InFlightFences           = std::move(inFlightFences);

        return storage;
    }

#undef CHECK_RETURN

    void Vulkan2DRendererAPI::DestroyComponents(Renderer2DStorage& storage)
    {
        storage.m_IndexBuffer.As<Vulkan::IndexBuffer>()->Destroy(m_Context->m_LogicalDevice);
        storage.m_VertexBuffer.As<Vulkan::VertexBuffer>()->Destroy(m_Context->m_LogicalDevice);
        for (std::size_t i = 0; i < storage.m_ImageAvailableSemaphores.size(); ++i)
        {
            storage.m_ImageAvailableSemaphores[i].As<Vulkan::Semaphore>()->Destroy(m_Context->m_LogicalDevice);
            storage.m_RenderFinishedSemaphores[i].As<Vulkan::Semaphore>()->Destroy(m_Context->m_LogicalDevice);
            storage.m_InFlightFences[i].As<Vulkan::Fence>()->Destroy(m_Context->m_LogicalDevice);
        }


        storage.m_GraphicsPipeline.As<Vulkan::GraphicsPipeline>()->Destroy(m_Context->m_LogicalDevice);
        storage.m_RenderPass.As<Vulkan::RenderPass>()->Destroy(m_Context->m_LogicalDevice);
        storage.m_FrameBuffers.As<Vulkan::SwapChainFrameBuffers>()->Destroy(m_Context->m_LogicalDevice);
        storage.m_ImageViews.As<Vulkan::ImageViews>()->Destroy(m_Context->m_LogicalDevice);

        auto commandPool = storage.m_CommandPool.As<Vulkan::CommandPool>();
        for (auto& commandBuffer : storage.m_CommandBuffers)
        {
            commandBuffer.As<Vulkan::CommandBuffer>()->Destroy(m_Context->m_LogicalDevice, *commandPool);
        }
        storage.m_CommandPool.As<Vulkan::CommandPool>()->Destroy(m_Context->m_LogicalDevice);
    }

    void Vulkan2DRendererAPI::SetViewport(RefPtr<CommandBuffer>& commandBuffer, Viewport& viewport)
    {
        VkViewport vkViewport {};
        vkViewport.x        = viewport.X;
        vkViewport.y        = viewport.Y;
        vkViewport.width    = viewport.Width;
        vkViewport.height   = viewport.Height;
        vkViewport.minDepth = viewport.MinDepth;
        vkViewport.maxDepth = viewport.MaxDepth;

        vkCmdSetViewport(commandBuffer.As<Vulkan::CommandBuffer>()->m_CommandBuffer, 0, 1, &vkViewport);
    }

    void Vulkan2DRendererAPI::SetScissor(RefPtr<CommandBuffer>& commandBuffer, Bounds& scissor)
    {
        VkRect2D vkScissor {};
        vkScissor.offset = {scissor.X, scissor.Y};
        vkScissor.extent = {static_cast<std::uint32_t>(scissor.Width), static_cast<std::uint32_t>(scissor.Height)};

        vkCmdSetScissor(commandBuffer.As<Vulkan::CommandBuffer>()->m_CommandBuffer, 0, 1, &vkScissor);
    }

    void Vulkan2DRendererAPI::AcquireNextImage(RefPtr<GraphicsContext>& context,
                                               RefPtr<Semaphore>& imageAvailableSemaphore,
                                               std::uint32_t& imageIndex)
    {
        auto ctx        = context.As<VulkanGraphicsContext>();
        VkResult result = vkAcquireNextImageKHR(
            ctx->m_LogicalDevice.GetHandle(), ctx->m_SwapChain.GetHandle(), std::numeric_limits<std::uint64_t>::max(),
            imageAvailableSemaphore.As<Vulkan::Semaphore>()->m_Semaphore, VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            m_NeedsResize = true;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            NEBULA_CORE_LOG_ERROR("Failed to acquire swap chain image!");
        }
    }

    void Vulkan2DRendererAPI::Present(std::uint32_t imageIndex, RefPtr<Semaphore>& renderingFinishedSemaphore)
    {
        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType            = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        std::array<VkSemaphore, 1> waitSemaphores = {renderingFinishedSemaphore.As<Vulkan::Semaphore>()->m_Semaphore};
        presentInfo.waitSemaphoreCount            = static_cast<std::uint32_t>(waitSemaphores.size());
        presentInfo.pWaitSemaphores               = waitSemaphores.data();

        std::array<VkSwapchainKHR, 1> swapChains = {m_Context->m_SwapChain.GetHandle()};
        presentInfo.swapchainCount               = static_cast<std::uint32_t>(swapChains.size());
        presentInfo.pSwapchains                  = swapChains.data();
        presentInfo.pImageIndices                = &imageIndex;

        VkResult result = vkQueuePresentKHR(m_Context->m_LogicalDevice.GetPresentQueue(), &presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            m_NeedsResize = true;
        }
        else if (result != VK_SUCCESS)
        {
            NEBULA_CORE_LOG_ERROR("Failed to present swap chain image!");
        }
    }

    void Vulkan2DRendererAPI::WaitDeviceIdle(RefPtr<GraphicsContext>& context)
    {
        auto ctx = context.As<VulkanGraphicsContext>();
        vkDeviceWaitIdle(ctx->m_LogicalDevice.GetHandle());
    }

    void Vulkan2DRendererAPI::DrawInstanced(RefPtr<CommandBuffer>& commandBuffer,
                                            std::uint32_t vertexCount,
                                            std::uint32_t instanceCount,
                                            std::uint32_t firstVertex,
                                            std::uint32_t firstInstance)
    {
        vkCmdDraw(commandBuffer.As<Vulkan::CommandBuffer>()->m_CommandBuffer, vertexCount, instanceCount, firstVertex,
                  firstInstance);
    }

    void Vulkan2DRendererAPI::DrawInstancedIndexed(RefPtr<CommandBuffer>& commandBuffer,
                                                   std::uint32_t indexCount,
                                                   std::uint32_t instanceCount,
                                                   std::uint32_t firstIndex,
                                                   std::uint32_t vertexOffset,
                                                   std::uint32_t firstInstance)
    {
        vkCmdDrawIndexed(commandBuffer.As<Vulkan::CommandBuffer>()->m_CommandBuffer, indexCount, instanceCount, firstIndex,
                         static_cast<std::int32_t>(vertexOffset), firstInstance);
    }

    Bounds Vulkan2DRendererAPI::GetSurfaceSize()
    {
        VkExtent2D extent = m_Context->m_SwapChain.GetExtent();
        return Bounds(0, 0, static_cast<std::int32_t>(extent.width), static_cast<std::int32_t>(extent.height));
    }

    void Vulkan2DRendererAPI::ResizeViewport(Renderer2DStorage& storage, Bounds& viewport)
    {
        auto& device = m_Context->m_LogicalDevice;
        vkDeviceWaitIdle(device.GetHandle());

        // We have to do in this order:
        // Framebuffers -> Color resources -> Depth resources -> Image views -> Swap chain
        storage.m_FrameBuffers.As<Vulkan::SwapChainFrameBuffers>()->Destroy(device);
        storage.m_ImageViews.As<Vulkan::ImageViews>()->Destroy(device);
        m_Context->m_SwapChain.Destroy(device);

        m_Context->m_SwapChain.Init(m_Context->m_PhysicalDevice, device, m_Context->m_Surface);
        storage.m_ImageViews.As<Vulkan::ImageViews>()->Init(device, m_Context->m_SwapChain);
        auto renderPass = storage.m_RenderPass.As<Vulkan::RenderPass>();
        auto imageViews = storage.m_ImageViews.As<Vulkan::ImageViews>();
        storage.m_FrameBuffers.As<Vulkan::SwapChainFrameBuffers>()->Init(device, *renderPass, *imageViews,
                                                                         m_Context->m_SwapChain);

        m_Viewport    = viewport;
        m_NeedsResize = false;
    }

    RefPtr<Vulkan2DRendererAPI> Vulkan2DRendererAPI::Create(RefPtr<VulkanGraphicsContext> context, Bounds viewport)
    {
        return RefPtr<Vulkan2DRendererAPI>::Create(context, viewport);
    }
} // namespace Nebula
