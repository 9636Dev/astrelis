#pragma once

#include <cstdint>

#include "NebulaEngine/Core/Pointer.hpp"

namespace Nebula
{
    class IndexBuffer
    {
    public:
        IndexBuffer()                               = default;
        virtual ~IndexBuffer()                      = default;
        IndexBuffer(const IndexBuffer&)            = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&&)                 = delete;
        IndexBuffer& operator=(IndexBuffer&&)      = delete;

        virtual void Bind() const   = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(const std::uint32_t* data, std::uint32_t count) = 0;
        virtual std::uint32_t GetCount() const = 0;

        static Ptr<IndexBuffer> Create(std::uint32_t count);
        static Ptr<IndexBuffer> Create(const std::uint32_t* data, std::uint32_t size);
    };
} // namespace Nebula
