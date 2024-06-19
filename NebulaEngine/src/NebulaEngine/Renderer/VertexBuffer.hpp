#pragma once

#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "GraphicsContext.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include <cstddef>

namespace Nebula
{
    class VertexBuffer
    {
    public:
        VertexBuffer() = default;
        virtual ~VertexBuffer()                      = default;
        VertexBuffer(const VertexBuffer&)            = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&)                 = delete;
        VertexBuffer& operator=(VertexBuffer&&)      = delete;

        virtual bool SetData(RefPtr<GraphicsContext>& context, RefPtr<CommandPool>& commandPool, const void* data, std::size_t size) = 0;
        virtual void Bind(RefPtr<CommandBuffer>& buffer) const                                     = 0;
    };
} // namespace Nebula
