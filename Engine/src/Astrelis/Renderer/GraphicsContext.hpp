#pragma once

#include "Astrelis/Core/Pointer.hpp"
#include "Astrelis/Core/Result.hpp"

#include <cstdint>

struct GLFWwindow;

namespace Astrelis
{

    struct ContextProps
    {
        bool VSync = true;
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

        virtual Result<EmptyType, std::string> Init() = 0;
        virtual void Shutdown()                       = 0;
        virtual bool IsInitialized() const            = 0;
        virtual void BeginFrame()                     = 0;
        virtual void EndFrame()                       = 0;
        virtual bool SkipFrame()                      = 0;

        virtual bool IsVSync() const        = 0;
        virtual void SetVSync(bool enabled) = 0;

        virtual std::uint32_t GetCurrentFrameIndex() const = 0;
        virtual std::uint32_t GetImageIndex() const        = 0;

        static RefPtr<GraphicsContext> Create(RawRef<GLFWwindow*> window, ContextProps props = {});
    };
} // namespace Astrelis
