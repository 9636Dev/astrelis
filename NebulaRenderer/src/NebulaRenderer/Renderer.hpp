#pragma once

#include "NebulaCore/Util/Result.hpp"
#include "NebulaWindowing/Window.hpp"

namespace Nebula
{
    // Really basic renderer, just drawing triangles
    class Renderer
    {
    public:
        Renderer() = default;
        virtual ~Renderer() = default;
        Renderer(const Renderer &) = delete;
        Renderer &operator=(const Renderer &) = delete;
        Renderer(Renderer &&) = delete;
        Renderer &operator=(Renderer &&) = delete;

        virtual void Clear() = 0;
    };

    enum class RendererError
    {
        None,
        FailedToInit,
        FailedToAttach, // Failed to attach to window
    };

    Result<Ptr<Renderer>, RendererError> CreateRenderer(Ref<Window> window);
} // namespace Nebula
