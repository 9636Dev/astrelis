#pragma once

#include "NebulaEngine/Core/Bounds.hpp"
#include "NebulaEngine/Core/Pointer.hpp"

#include "GraphicsContext.hpp"
#include "NebulaEngine/Renderer/RendererStorage.hpp"

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

        virtual Renderer2DStorage CreateComponents() = 0;
        virtual void DestroyComponents(Renderer2DStorage& storage) = 0;

        static RefPtr<RendererAPI>
            Create(RefPtr<GraphicsContext> context, Bounds viewport, Type type = Type::Renderer2D);
    private:
        static API s_API;
    };

} // namespace Nebula
