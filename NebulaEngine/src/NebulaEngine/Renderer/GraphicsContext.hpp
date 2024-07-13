#pragma once

#include <cstdint>

#include "NebulaEngine/Core/Pointer.hpp"

struct GLFWwindow;

namespace Nebula
{

    struct ContextProps
    {
        std::uint32_t MaxFramesInFlight = 2;
    };

    /**
    * @brief Per window graphics context, contains per window rendering state and resources.
    */
    class GraphicsContext
    {
    public:
        GraphicsContext()                                  = default;
        virtual ~GraphicsContext()                         = default;
        GraphicsContext(const GraphicsContext&)            = delete;
        GraphicsContext& operator=(const GraphicsContext&) = delete;
        GraphicsContext(GraphicsContext&&)                 = delete;
        GraphicsContext& operator=(GraphicsContext&&)      = delete;

        virtual bool Init()                = 0;
        virtual void Shutdown()            = 0;
        virtual bool IsInitialized() const = 0;
        virtual void BeginFrame()          = 0;
        virtual void EndFrame()            = 0;
        virtual bool SkipFrame()           = 0;

        virtual std::uint32_t GetCurrentFrameIndex() const = 0;
        virtual std::uint32_t GetImageIndex() const        = 0;

        static RefPtr<GraphicsContext> Create(RawRef<GLFWwindow*> window, ContextProps props = {});
    };
} // namespace Nebula
