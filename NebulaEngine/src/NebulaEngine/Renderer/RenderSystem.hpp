#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Window.hpp"

namespace Nebula
{
    class RenderSystem
    {
    public:
        RenderSystem()                               = default;
        virtual ~RenderSystem()                      = default;
        RenderSystem(const RenderSystem&)            = default;
        RenderSystem(RenderSystem&&)                 = default;
        RenderSystem& operator=(const RenderSystem&) = default;
        RenderSystem& operator=(RenderSystem&&)      = default;

        virtual bool Init()                    = 0;
        virtual void Shutdown()                = 0;
        virtual void StartGraphicsRenderPass() = 0;
        virtual void EndGraphicsRenderPass()   = 0;
        virtual void BlitSwapchain()           = 0;
        virtual void EndFrame()                = 0;

        static RefPtr<RenderSystem> Create(RefPtr<Window>& window);
    };
} // namespace Nebula
