#pragma once

#include <cstdint>
namespace Nebula::OpenGL
{
    class VertexBuffer;
    class VertexBufferFormat;

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();
        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray(VertexArray&& other) noexcept;
        VertexArray& operator=(VertexArray&& other) noexcept;

        void AddVertexBuffer(const VertexBuffer& vertexBuffer, const VertexBufferFormat& format);

        void Bind() const;
        void Unbind() const;
    private:
        std::uint32_t m_Id;
    };
} // namespace Nebula::OpenGL
