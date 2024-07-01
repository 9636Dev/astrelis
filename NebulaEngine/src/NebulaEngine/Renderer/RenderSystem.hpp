#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Window.hpp"
namespace Nebula
{
    class RenderSystem
    {
    public:
        RenderSystem() = default;
        virtual ~RenderSystem() = default;
        RenderSystem(const RenderSystem&) = default;
        RenderSystem(RenderSystem&&) = default;
        RenderSystem& operator=(const RenderSystem&) = default;
        RenderSystem& operator=(RenderSystem&&) = default;

        virtual void StartGraphicsRenderPass() = 0;
        virtual void EndGraphicsRenderPass() = 0;
        virtual void StartOverlayRenderPass() = 0;
        virtual void EndOverlayRenderPass() = 0;

        RefPtr<RenderSystem> Create(RefPtr<Window>& window);
    };
} // namespace Nebula
