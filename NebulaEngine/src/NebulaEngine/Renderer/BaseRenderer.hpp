#pragma once

#include "GraphicsContext.hpp"
#include "GraphicsPipeline.hpp"
#include "RendererAPI.hpp"

#include "NebulaEngine/Core/Window.hpp"

namespace Nebula
{
    class BaseRenderer
    {
    public:
        BaseRenderer(RefPtr<Window> window, Rect2Di viewport);
        virtual ~BaseRenderer()                      = default;
        BaseRenderer(const BaseRenderer&)            = delete;
        BaseRenderer& operator=(const BaseRenderer&) = delete;
        BaseRenderer(BaseRenderer&&)                 = delete;
        BaseRenderer& operator=(BaseRenderer&&)      = delete;

        bool Init();
        virtual void Shutdown();

        virtual void BeginFrame() = 0;
        virtual void EndFrame()   = 0;

        virtual void ResizeViewport();
    protected:
        void InitAPI(); // THis is private, and not virtual
        virtual bool InitComponents() = 0;
        void InternalBeginFrame();

        RefPtr<Window> m_Window;
        RefPtr<GraphicsContext> m_Context;
        RefPtr<RendererAPI> m_RendererAPI;

        RefPtr<GraphicsPipeline> m_Pipeline;
    };
} // namespace Nebula
