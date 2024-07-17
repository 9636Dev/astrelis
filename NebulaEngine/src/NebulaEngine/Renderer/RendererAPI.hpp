#pragma once

#include "NebulaEngine/Core/Bounds.hpp"
#include "NebulaEngine/Core/Math.hpp"
#include "NebulaEngine/Core/Pointer.hpp"

#include "DescriptorSetLayout.hpp"
#include "DescriptorSets.hpp"
#include "GraphicsContext.hpp"
#include "GraphicsPipeline.hpp"
#include "IndexBuffer.hpp"
#include "TextureImage.hpp"
#include "TextureSampler.hpp"
#include "UniformBuffer.hpp"
#include "VertexBuffer.hpp"
#include "Viewport.hpp"

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
        enum class BufferingMode
        {
            Single,
            Double,
            Triple,
        };

        static API GetAPI() { return s_API; }

        /**
        * @brief Set the API to be used by the renderer, only works before the App is created
        */
        static void SetAPI(API api) { s_API = api; }

        static BufferingMode GetBufferingMode() { return s_BufferingMode; }

        static std::uint32_t GetBufferingCount()
        {
            switch (s_BufferingMode)
            {
            case BufferingMode::Single:
                return 1;
            case BufferingMode::Double:
                return 2;
            case BufferingMode::Triple:
                return 3;
            }
            return 0;
        }

        static void SetBufferingMode(BufferingMode mode) { s_BufferingMode = mode; }

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

        virtual void SetViewport(Viewport& viewport) = 0;
        virtual void SetScissor(Bounds& scissor)     = 0;

        virtual void WaitDeviceIdle()                     = 0;
        virtual Bounds GetSurfaceSize()                   = 0;
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

        virtual RefPtr<GraphicsPipeline> CreateGraphicsPipeline() = 0;
        // TODO: Maybe UI Pipeline? Distinguish between graphics and UI
        virtual RefPtr<VertexBuffer> CreateVertexBuffer()               = 0;
        virtual RefPtr<IndexBuffer> CreateIndexBuffer()                 = 0;
        virtual RefPtr<DescriptorSetLayout> CreateDescriptorSetLayout() = 0;
        virtual RefPtr<DescriptorSets> CreateDescriptorSets()           = 0;
        virtual RefPtr<UniformBuffer> CreateUniformBuffer()             = 0;
        virtual RefPtr<TextureImage> CreateTextureImage()               = 0;
        virtual RefPtr<TextureSampler> CreateTextureSampler()           = 0;

        static RefPtr<RendererAPI>
            Create(RefPtr<GraphicsContext> context, Bounds viewport, Type type = Type::Renderer2D);
    private:
        static API s_API;
        static BufferingMode s_BufferingMode;
    };

} // namespace Nebula
