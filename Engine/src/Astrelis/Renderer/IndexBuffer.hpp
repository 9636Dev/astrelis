#pragma once

#include <cstdint>

#include "GraphicsContext.hpp"

namespace Astrelis
{
    // uint32_t is used
    class IndexBuffer
    {
    public:
        IndexBuffer()                              = default;
        virtual ~IndexBuffer()                     = default;
        IndexBuffer(const IndexBuffer&)            = default;
        IndexBuffer& operator=(const IndexBuffer&) = default;
        IndexBuffer(IndexBuffer&&)                 = default;
        IndexBuffer& operator=(IndexBuffer&&)      = default;

        virtual bool Init(RefPtr<GraphicsContext>& context, std::uint32_t count)                               = 0;
        virtual void Destroy(RefPtr<GraphicsContext>& context)                                                 = 0;
        virtual bool SetData(RefPtr<GraphicsContext>& context, const std::uint32_t* data, std::uint32_t count) = 0;
        virtual void Bind(RefPtr<GraphicsContext>& buffer) const                                               = 0;
    };
} // namespace Astrelis
