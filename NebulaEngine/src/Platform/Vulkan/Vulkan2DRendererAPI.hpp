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

        void SetViewport(Viewport& viewport) override;
        void SetScissor(Bounds& scissor) override;
        void WaitDeviceIdle() override;
        void DrawInstanced(std::uint32_t vertexCount, std::uint32_t instanceCount, std::uint32_t firstVertex, std::uint32_t firstInstance) override;
        void DrawInstancedIndexed(std::uint32_t indexCount, std::uint32_t instanceCount, std::uint32_t firstIndex, std::uint32_t vertexOffset, std::uint32_t firstInstance) override;
        Bounds GetSurfaceSize() override;

        void ResizeViewport(Renderer2DStorage& storage, Bounds& viewport) override;
        bool NeedsResize() const override { return m_Context->m_NeedsResize; }

        static RefPtr<Vulkan2DRendererAPI> Create(RefPtr<VulkanGraphicsContext> context, Bounds viewport);
    private:
        RefPtr<VulkanGraphicsContext> m_Context;

        Bounds m_Viewport;
    };
} // namespace Nebula
