#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Window.hpp"
#include "NebulaEngine/IO/Image.hpp"
#include <future>

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

        virtual void SetBlitSwapchain(bool blit) = 0;
        virtual void* GetGraphicsImage()         = 0;

        /**
         * @brief Capture the current frame (next finished frame) and return it as an InMemoryImage
         * @return std::future<InMemoryImage> A future that will contain the InMemoryImage when the frame is captured
        */
        virtual std::future<InMemoryImage> CaptureFrame() = 0;

        static RefPtr<RenderSystem> Create(RefPtr<Window>& window);
    };
} // namespace Nebula
