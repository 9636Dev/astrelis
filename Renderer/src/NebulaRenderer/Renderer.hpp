#pragma once

#include "NebulaCore/Util/Pointer.hpp"
#include "NebulaCore/Util/Result.hpp"
#include "Windowing/Window.hpp"
#include "Geometry/Dimension.hpp"

namespace Nebula
{
    class Renderer
    {
    public:
        Renderer()                           = default;
        Renderer(const Renderer&)            = delete;
        Renderer(Renderer&&)                 = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&)      = delete;
        virtual ~Renderer()                  = default;

        virtual void Resize(Dimension dimensions)                                  = 0;

        virtual void SetClearColor(float red, float green, float blue, float alpha) = 0;
        virtual void NewFrame()                                                     = 0;
        virtual void Render()                                                       = 0;
    };

    enum class RendererCreationError
    {
        RendererCreationFailed
    };

    struct RenderingContext
    {
        Ref<Window> Window;
        Dimension   Dimensions;
    };

    extern Result<Ptr<Renderer>, RendererCreationError> CreateRenderer(const RenderingContext& context);
} // namespace Nebula
