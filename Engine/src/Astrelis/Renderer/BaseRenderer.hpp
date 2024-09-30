#pragma once

#include "Astrelis/Core/Window.hpp"

#include "GraphicsContext.hpp"
#include "GraphicsPipeline.hpp"
#include "RendererAPI.hpp"

namespace Astrelis {
    class BaseRenderer {
    public:
        BaseRenderer(RefPtr<Window> window, Rect2Di viewport);
        virtual ~BaseRenderer()                      = default;
        BaseRenderer(const BaseRenderer&)            = delete;
        BaseRenderer& operator=(const BaseRenderer&) = delete;
        BaseRenderer(BaseRenderer&&)                 = delete;
        BaseRenderer& operator=(BaseRenderer&&)      = delete;

        bool         Init();
        virtual void Shutdown();

        virtual void BeginFrame() = 0;
        virtual void EndFrame()   = 0;

        virtual void ResizeViewport();
    protected:
        /// @brief Initialize the API, this is already called in the Init function of the base class
        /// @note This function is called before the InitComponents function
        void InitAPI();
        /// @brief Initialize the components of the renderer
        /// @note This function is called after the InitAPI function
        virtual bool InitComponents() = 0;

        /// @brief Begin the frame, this is already called in the BeginFrame function of the base class
        /// @note This function is called before the EndFrame function
        void InternalBeginFrame();

        RefPtr<Window>          m_Window;
        RefPtr<GraphicsContext> m_Context;
        RefPtr<RendererAPI>     m_RendererAPI;

        RefPtr<GraphicsPipeline> m_Pipeline;
    };
} // namespace Astrelis
