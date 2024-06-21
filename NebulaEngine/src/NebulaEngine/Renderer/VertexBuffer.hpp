#pragma once

#include "GraphicsContext.hpp"
#include "NebulaEngine/Core/Pointer.hpp"
#include <cstddef>

namespace Nebula
{
    class VertexBuffer
    {
    public:
        VertexBuffer()                               = default;
        virtual ~VertexBuffer()                      = default;
        VertexBuffer(const VertexBuffer&)            = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&)                 = delete;
        VertexBuffer& operator=(VertexBuffer&&)      = delete;

        virtual bool Init(RefPtr<GraphicsContext>& context, std::size_t size) = 0;
        virtual void Destroy(RefPtr<GraphicsContext>& context)                = 0;

        virtual bool SetData(RefPtr<GraphicsContext>& context, const void* data, std::size_t size) = 0;
        virtual void Bind(RefPtr<GraphicsContext>& context, std::uint32_t binding) const           = 0;
    };
} // namespace Nebula
