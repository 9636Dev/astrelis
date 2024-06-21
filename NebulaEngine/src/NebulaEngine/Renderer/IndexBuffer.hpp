#pragma once

#include "GraphicsContext.hpp"
#include <cstddef>

namespace Nebula
{
    class IndexBuffer
    {
    public:
        IndexBuffer()                              = default;
        virtual ~IndexBuffer()                     = default;
        IndexBuffer(const IndexBuffer&)            = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&&)                 = delete;
        IndexBuffer& operator=(IndexBuffer&&)      = delete;

        virtual bool SetData(RefPtr<GraphicsContext>& context, const std::uint32_t* data, std::uint32_t count) = 0;
        virtual void Bind(RefPtr<GraphicsContext>& buffer) const                                               = 0;
    };
} // namespace Nebula
