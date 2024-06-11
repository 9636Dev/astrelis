#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
namespace Nebula
{
    class RenderContext
    {
    public:
        RenderContext() = default;
        virtual ~RenderContext() = default;
        RenderContext(const RenderContext&) = delete;
        RenderContext& operator=(const RenderContext&) = delete;
        RenderContext(RenderContext&&) = delete;
        RenderContext& operator=(RenderContext&&) = delete;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;

        /**
        * @brief Create a new RenderContext, depends on Renderer::GetAPI()
        * @param window The window to create the context for
        * @return A pointer to the created RenderContext (can be nullptr)
        * @see Renderer::GetAPI()
        */
        static Ptr<RenderContext> Create(void* window);
    };
} // namespace Nebula
