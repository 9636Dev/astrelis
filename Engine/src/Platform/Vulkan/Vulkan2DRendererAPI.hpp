#pragma once

#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Renderer/RendererAPI.hpp"
#include "Astrelis/Renderer/UniformBuffer.hpp"

#include "Platform/Vulkan/VulkanGraphicsContext.hpp"

namespace Astrelis {
    class Vulkan2DRendererAPI : public RendererAPI {
    public:
        explicit Vulkan2DRendererAPI(RefPtr<VulkanGraphicsContext> context);
        ~Vulkan2DRendererAPI() override                            = default;
        Vulkan2DRendererAPI(const Vulkan2DRendererAPI&)            = delete;
        Vulkan2DRendererAPI& operator=(const Vulkan2DRendererAPI&) = delete;
        Vulkan2DRendererAPI(Vulkan2DRendererAPI&&)                 = delete;
        Vulkan2DRendererAPI& operator=(Vulkan2DRendererAPI&&)      = delete;

        void Init() override;
        void Shutdown() override;

        void    SetViewport(Rect3Df& viewport) override;
        void    SetScissor(Rect2Di& scissor) override;
        void    WaitDeviceIdle() override;
        void    DrawInstanced(std::uint32_t vertexCount, std::uint32_t instanceCount,
               std::uint32_t firstVertex, std::uint32_t firstInstance) override;
        void    DrawInstancedIndexed(std::uint32_t indexCount, std::uint32_t instanceCount,
               std::uint32_t firstIndex, std::uint32_t vertexOffset,
               std::uint32_t firstInstance) override;
        Rect2Di GetSurfaceSize() override;

        void ResizeViewport() override {
            m_Context->m_SwapchainRecreation = true;
        }

        bool NeedsResize() const override {
            return m_Context->m_SwapchainRecreation;
        }

        void CorrectProjection(Mat4f& projection) override;

        RefPtr<GraphicsPipeline>     CreateGraphicsPipeline() override;
        RefPtr<VertexBuffer>         CreateVertexBuffer() override;
        RefPtr<IndexBuffer>          CreateIndexBuffer() override;
        RefPtr<BindingDescriptorSet> CreateBindingDescriptorSet(
            BindingDescriptorSet::Mode mode) override;
        RefPtr<UniformBuffer>  CreateUniformBuffer() override;
        RefPtr<TextureImage>   CreateTextureImage() override;
        RefPtr<TextureSampler> CreateTextureSampler() override;

        static RefPtr<Vulkan2DRendererAPI> Create(RefPtr<VulkanGraphicsContext> context);
    private:
        RefPtr<VulkanGraphicsContext> m_Context;
    };
} // namespace Astrelis
