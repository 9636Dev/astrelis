#pragma once

#include "NebulaEngine/Core/Bounds.hpp"
#include "NebulaEngine/Core/Pointer.hpp"

#include "DescriptorSetLayout.hpp"
#include "DescriptorSets.hpp"
#include "GraphicsContext.hpp"
#include "GraphicsPipeline.hpp"
#include "RendererStorage.hpp"
#include "TextureImage.hpp"
#include "TextureSampler.hpp"
#include "UniformBuffer.hpp"
#include "VertexBuffer.hpp"
#include "Viewport.hpp"

#include <glm/glm.hpp>

namespace Nebula
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None   = 0,
            Vulkan = 1,
        };
        enum class Type
        {
            Renderer2D,
            Renderer3D,
            RayTracer,
        };

        static API GetAPI() { return s_API; }

        RendererAPI()                              = default;
        virtual ~RendererAPI()                     = default;
        RendererAPI(const RendererAPI&)            = delete;
        RendererAPI& operator=(const RendererAPI&) = delete;
        RendererAPI(RendererAPI&&)                 = delete;
        RendererAPI& operator=(RendererAPI&&)      = delete;

        virtual void Init()     = 0;
        virtual void Shutdown() = 0;

        struct CreateDetails
        {
            std::size_t VertexBufferSize = 0;
            std::uint32_t IndicesCount   = 0;
            std::vector<BufferBinding> VertexInput;
            std::vector<RefPtr<DescriptorSetLayout>> DescriptorSetLayouts;
        };

        virtual Renderer2DStorage CreateComponents(CreateDetails& details) = 0;
        virtual void DestroyComponents(Renderer2DStorage& storage)         = 0;

        virtual void SetViewport(Viewport& viewport) = 0;
        virtual void SetScissor(Bounds& scissor)     = 0;

        virtual void WaitDeviceIdle()                         = 0;
        virtual Bounds GetSurfaceSize()                       = 0;
        virtual void CorrectProjection(glm::mat4& projection) = 0;

        // Probably need to recreate a lot of things, so we need to pass in the storage
        virtual void ResizeViewport()    = 0;
        virtual bool NeedsResize() const = 0;

        virtual void DrawInstanced(std::uint32_t vertexCount,
                                   std::uint32_t instanceCount,
                                   std::uint32_t firstVertex,
                                   std::uint32_t firstInstance)        = 0;
        virtual void DrawInstancedIndexed(std::uint32_t indexCount,
                                          std::uint32_t instanceCount,
                                          std::uint32_t firstIndex,
                                          std::uint32_t vertexOffset,
                                          std::uint32_t firstInstance) = 0;

        virtual RefPtr<VertexBuffer> CreateVertexBuffer()               = 0;
        virtual RefPtr<DescriptorSetLayout> CreateDescriptorSetLayout() = 0;
        virtual RefPtr<DescriptorSets> CreateDescriptorSets()           = 0;
        virtual RefPtr<UniformBuffer> CreateUniformBuffer()             = 0;
        virtual RefPtr<TextureImage> CreateTextureImage()               = 0;
        virtual RefPtr<TextureSampler> CreateTextureSampler()           = 0;

        static RefPtr<RendererAPI>
            Create(RefPtr<GraphicsContext> context, Bounds viewport, Type type = Type::Renderer2D);
    private:
        static API s_API;
    };

} // namespace Nebula
