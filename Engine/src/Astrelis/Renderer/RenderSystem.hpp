#pragma once

#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Core/Window.hpp"
#include "Astrelis/IO/Image.hpp"

#include <future>

namespace Astrelis {
    struct FrameCaptureProps {
        std::uint32_t Width;
        std::uint32_t Height;
    };

    class RenderSystem {
    public:
        RenderSystem()                               = default;
        virtual ~RenderSystem()                      = default;
        RenderSystem(const RenderSystem&)            = default;
        RenderSystem(RenderSystem&&)                 = default;
        RenderSystem& operator=(const RenderSystem&) = default;
        RenderSystem& operator=(RenderSystem&&)      = default;

        virtual bool    Init()                                     = 0;
        virtual void    Shutdown()                                 = 0;
        virtual void    StartGraphicsRenderPass()                  = 0;
        virtual void    EndGraphicsRenderPass()                    = 0;
        virtual Rect2Du GetGraphicsRenderArea()                    = 0;
        virtual void    SetGraphicsRenderArea(const Rect2Du& area) = 0;
        virtual void    BlitSwapchain()                            = 0;
        virtual void    EndFrame()                                 = 0;

        virtual void    SetBlitSwapchain(bool blit) = 0;
        virtual void*   GetGraphicsImage()          = 0;
        virtual Rect2Di GetRenderBounds()           = 0;

        /**
         * @brief Capture the current frame (next finished frame) and return it as an InMemoryImage
         * @return std::future<InMemoryImage> A future that will contain the InMemoryImage when the frame is captured
        */
        virtual std::future<InMemoryImage> CaptureFrame(const FrameCaptureProps& props) = 0;

        static RefPtr<RenderSystem> Create(RefPtr<Window>& window);
    };
} // namespace Astrelis
