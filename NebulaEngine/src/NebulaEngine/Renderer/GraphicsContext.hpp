#pragma once

#include "NebulaEngine/Core/Pointer.hpp"

namespace Nebula
{
    class GraphicsContext
    {
    public:
        GraphicsContext() = default;
        virtual ~GraphicsContext() = default;
        GraphicsContext(const GraphicsContext &) = delete;
        GraphicsContext &operator=(const GraphicsContext &) = delete;
        GraphicsContext(GraphicsContext &&) = delete;
        GraphicsContext &operator=(GraphicsContext &&) = delete;

        virtual bool Init() = 0;
        virtual void Shutdown() = 0;

        static RefPtr<GraphicsContext> Create();
    };
} // namespace Nebula
