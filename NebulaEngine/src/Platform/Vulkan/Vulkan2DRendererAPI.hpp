#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Renderer/RendererAPI.hpp"
#include "NebulaEngine/Renderer/UniformBuffer.hpp"
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

        void ResizeViewport() override { m_Context->m_NeedsResize = true; }
        bool NeedsResize() const override { return m_Context->m_NeedsResize; }
        void CorrectProjection(glm::mat4& projection) override;

        RefPtr<VertexBuffer> CreateVertexBuffer() override;
        RefPtr<IndexBuffer> CreateIndexBuffer() override;
        RefPtr<DescriptorSetLayout> CreateDescriptorSetLayout() override;
        RefPtr<DescriptorSets> CreateDescriptorSets() override;
        RefPtr<UniformBuffer> CreateUniformBuffer() override;
        RefPtr<TextureImage> CreateTextureImage() override;
        RefPtr<TextureSampler> CreateTextureSampler() override;

        static RefPtr<Vulkan2DRendererAPI> Create(RefPtr<VulkanGraphicsContext> context, Bounds viewport);
    private:
        RefPtr<VulkanGraphicsContext> m_Context;

        Bounds m_Viewport;
    };
} // namespace Nebula
