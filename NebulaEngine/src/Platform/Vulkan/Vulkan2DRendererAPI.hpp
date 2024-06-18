#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Renderer/RendererAPI.hpp"
#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

namespace Nebula
{
    class Vulkan2DRendererAPI : public RendererAPI
    {
    public:
        Vulkan2DRendererAPI(RefPtr<VulkanGraphicsContext> context, Bounds viewport);
        ~Vulkan2DRendererAPI() override                            = default;
        Vulkan2DRendererAPI(const Vulkan2DRendererAPI&)            = delete;
        Vulkan2DRendererAPI& operator=(const Vulkan2DRendererAPI&) = delete;
        Vulkan2DRendererAPI(Vulkan2DRendererAPI&&)                 = delete;
        Vulkan2DRendererAPI& operator=(Vulkan2DRendererAPI&&)      = delete;

        void Init() override;
        void Shutdown() override;

        Renderer2DStorage CreateComponents(CreateDetails& details) override;
        void DestroyComponents(Renderer2DStorage& storage) override;

        void SetViewport(RefPtr<CommandBuffer>& commandBuffer, Viewport& viewport) override;
        void SetScissor(RefPtr<CommandBuffer>& commandBuffer, Bounds& scissor) override;
        void AcquireNextImage(RefPtr<GraphicsContext>& context, RefPtr<Semaphore>& imageAvailableSemaphore, std::uint32_t& imageIndex) override;
        void Present(std::uint32_t imageIndex, RefPtr<Semaphore>& renderingFinishedSemaphore) override;
        void WaitDeviceIdle(RefPtr<GraphicsContext>& context) override;
        void DrawInstanced(RefPtr<CommandBuffer>& commandBuffer, std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex, std::uint32_t firstInstance) override;
        Bounds GetSurfaceSize() override;

        void ResizeViewport(Renderer2DStorage& storage, Bounds& viewport) override;
        bool NeedsResize() const override { return m_NeedsResize; }

        static RefPtr<Vulkan2DRendererAPI> Create(RefPtr<VulkanGraphicsContext> context, Bounds viewport);
    private:
        RefPtr<VulkanGraphicsContext> m_Context;
        Bounds m_Viewport;
        bool m_NeedsResize = false;
    };
} // namespace Nebula
