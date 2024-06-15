#pragma once

#include <cstdint>

#include "NebulaEngine/Core/Pointer.hpp"

namespace Nebula
{
    class VertexBuffer
    {
    public:
        VertexBuffer() = default;
        virtual ~VertexBuffer() = default;
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&) = delete;
        VertexBuffer& operator=(VertexBuffer&&) = delete;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(const void* data, std::uint32_t size) = 0;

        static Ptr<VertexBuffer> Create(float* vertices, std::uint32_t size);
    };
} // namespace Nebula
