#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Renderer/IndexBuffer.hpp"
#include "NebulaEngine/Renderer/VertexBuffer.hpp"

namespace Nebula
{
    class VertexArray
    {
    public:
        VertexArray() noexcept                     = default;
        virtual ~VertexArray()                     = default;
        VertexArray(const VertexArray&)            = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray(VertexArray&&)                 = delete;
        VertexArray& operator=(VertexArray&&)      = delete;

        virtual void Bind() const   = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const Ptr<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(Ref<IndexBuffer> indexBuffer)    = 0;

        virtual Ref<IndexBuffer>& GetIndexBuffer() = 0;
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

        static Ptr<VertexArray> Create();
    };
} // namespace Nebula
