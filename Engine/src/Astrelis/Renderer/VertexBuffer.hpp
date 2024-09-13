#pragma once

#include "Astrelis/Core/Pointer.hpp"

#include <cstddef>

#include "GraphicsContext.hpp"

namespace Astrelis
{
    class VertexBuffer
    {
    public:
        VertexBuffer()                               = default;
        virtual ~VertexBuffer()                      = default;
        VertexBuffer(const VertexBuffer&)            = default;
        VertexBuffer& operator=(const VertexBuffer&) = default;
        VertexBuffer(VertexBuffer&&)                 = default;
        VertexBuffer& operator=(VertexBuffer&&)      = default;

        virtual bool Init(RefPtr<GraphicsContext>& context, std::size_t size) = 0;
        virtual void Destroy(RefPtr<GraphicsContext>& context)                = 0;

        virtual bool SetData(RefPtr<GraphicsContext>& context, const void* data, std::size_t size) = 0;
        virtual void Bind(RefPtr<GraphicsContext>& context, std::uint32_t binding) const           = 0;
    };
} // namespace Astrelis
