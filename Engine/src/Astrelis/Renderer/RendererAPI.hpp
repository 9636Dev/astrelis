#pragma once

#include "DescriptorSetLayout.hpp"
#include "DescriptorSets.hpp"
#include "GraphicsContext.hpp"
#include "GraphicsPipeline.hpp"
#include "IndexBuffer.hpp"
#include "TextureImage.hpp"
#include "TextureSampler.hpp"
#include "UniformBuffer.hpp"
#include "VertexBuffer.hpp"

#include "Astrelis/Core/Geometry.hpp"
#include "Astrelis/Core/Math.hpp"
#include "Astrelis/Core/Pointer.hpp"

namespace Astrelis
{
    class RendererAPI
    {
    public:
        enum class API : std::uint8_t
        {
            None   = 0,
            Vulkan = 1,
        };
        enum class Type : std::uint8_t
        {
            Renderer2D,
            Renderer3D,
            RayTracer,
        };
        enum class BufferingMode : std::uint8_t
        {
            Single = 1,
            Double = 2,
            Triple = 3,
        };

        static API GetAPI() { return s_API; }

        /**
        * @brief Set the API to be used by the renderer, only works before the App is created
        */
        static void SetAPI(API api);

        static BufferingMode GetBufferingMode() { return s_BufferingMode; }

        static std::uint32_t GetBufferingCount() { return static_cast<std::uint32_t>(s_BufferingMode); }

        static void SetBufferingMode(BufferingMode mode);

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
            std::vector<BufferBinding> VertexInput;
            std::vector<RefPtr<DescriptorSetLayout>> DescriptorSetLayouts;
        };

        virtual void SetViewport(Rect3Df& viewport) = 0;
        virtual void SetScissor(Rect2Di& scissor)   = 0;

        virtual void WaitDeviceIdle()                     = 0;
        virtual Rect2Di GetSurfaceSize()                  = 0;
        virtual void CorrectProjection(Mat4f& projection) = 0;

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

        virtual RefPtr<GraphicsPipeline> CreateGraphicsPipeline()       = 0;
        virtual RefPtr<VertexBuffer> CreateVertexBuffer()               = 0;
        virtual RefPtr<IndexBuffer> CreateIndexBuffer()                 = 0;
        virtual RefPtr<DescriptorSetLayout> CreateDescriptorSetLayout() = 0;
        virtual RefPtr<DescriptorSets> CreateDescriptorSets()           = 0;
        virtual RefPtr<UniformBuffer> CreateUniformBuffer()             = 0;
        virtual RefPtr<TextureImage> CreateTextureImage()               = 0;
        virtual RefPtr<TextureSampler> CreateTextureSampler()           = 0;

        static RefPtr<RendererAPI> Create(RefPtr<GraphicsContext> context, Type type = Type::Renderer2D);
    private:
        static API s_API;
        static BufferingMode s_BufferingMode;
    };

} // namespace Astrelis
