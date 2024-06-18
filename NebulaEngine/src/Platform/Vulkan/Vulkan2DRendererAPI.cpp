#include "Vulkan2DRendererAPI.hpp"

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Core/Bounds.hpp"

#include "VK/CommandBuffer.hpp"
#include "VK/CommandPool.hpp"
#include "VK/GraphicsPipeline.hpp"
#include "VK/ImageViews.hpp"
#include "VK/RenderPass.hpp"
#include "VK/SwapChainFrameBuffers.hpp"
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

    Renderer2DStorage Vulkan2DRendererAPI::CreateComponents()
    {
        Renderer2DStorage storage;

        RefPtr<Vulkan::CommandPool> commandPool = RefPtr<Vulkan::CommandPool>::Create();
        commandPool->Init(m_Context->m_LogicalDevice);
        RefPtr<Vulkan::CommandBuffer> commandBuffer = RefPtr<Vulkan::CommandBuffer>::Create();
        commandBuffer->Init(m_Context->m_LogicalDevice, *commandPool);
        RefPtr<Vulkan::ImageViews> imageViews = RefPtr<Vulkan::ImageViews>::Create();
        imageViews->Init(m_Context->m_LogicalDevice, m_Context->m_SwapChain);
        RefPtr<Vulkan::RenderPass> renderPass = RefPtr<Vulkan::RenderPass>::Create();
        renderPass->Init(m_Context->m_LogicalDevice, m_Context->m_SwapChain);
        RefPtr<Vulkan::SwapChainFrameBuffers> frameBuffers = RefPtr<Vulkan::SwapChainFrameBuffers>::Create();
        frameBuffers->Init(m_Context->m_LogicalDevice, *renderPass, *imageViews, m_Context->m_SwapChain);
        RefPtr<Vulkan::GraphicsPipeline> graphicsPipeline = RefPtr<Vulkan::GraphicsPipeline>::Create();
        graphicsPipeline->Init(m_Context->m_LogicalDevice, *renderPass, m_Context->m_SwapChain);

        return Renderer2DStorage {
            static_cast<RefPtr<CommandPool>>(commandPool), static_cast<RefPtr<CommandBuffer>>(commandBuffer),
            static_cast<RefPtr<ImageViews>>(imageViews), static_cast<RefPtr<RenderPass>>(renderPass),
            static_cast<RefPtr<SwapChainFrameBuffers>>(frameBuffers),
            static_cast<RefPtr<GraphicsPipeline>>(graphicsPipeline)};
    }

    void Vulkan2DRendererAPI::DestroyComponents(Renderer2DStorage& storage)
    {
        storage.m_GraphicsPipeline.As<Vulkan::GraphicsPipeline>()->Destroy(m_Context->m_LogicalDevice);
        storage.m_RenderPass.As<Vulkan::RenderPass>()->Destroy(m_Context->m_LogicalDevice);
        storage.m_ImageViews.As<Vulkan::ImageViews>()->Destroy(m_Context->m_LogicalDevice);
        storage.m_CommandBuffer.As<Vulkan::CommandBuffer>()->Destroy(m_Context->m_LogicalDevice, *storage.m_CommandPool.As<Vulkan::CommandPool>());
        storage.m_CommandPool.As<Vulkan::CommandPool>()->Destroy(m_Context->m_LogicalDevice);
    }

    RefPtr<Vulkan2DRendererAPI> Vulkan2DRendererAPI::Create(RefPtr<VulkanGraphicsContext> context, Bounds viewport)
    {
        return RefPtr<Vulkan2DRendererAPI>::Create(context, viewport);
    }
} // namespace Nebula
