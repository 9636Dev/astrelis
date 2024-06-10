#include "VertexBuffer.hpp"

#include "GL.hpp"

namespace Nebula::OpenGL
{
    VertexBuffer::VertexBuffer()
    : m_Id(0)
    {
        GL::GenBuffers(1, &m_Id);
    }

    VertexBuffer::~VertexBuffer()
    {
        GL::DeleteBuffers(1, &m_Id);
    }

    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
        : m_Id(other.m_Id)
    {
        other.m_Id = 0;
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
    {
        if (this != &other)
        {
            GL::DeleteBuffers(1, &m_Id);
            m_Id = other.m_Id;
            other.m_Id = 0;
        }
        return *this;
    }

    void VertexBuffer::Bind() const
    {
        GL::BindBuffer(BufferTarget::ArrayBuffer, m_Id);
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void VertexBuffer::Unbind() const
    {
        GL::BindBuffer(BufferTarget::ArrayBuffer, 0);
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-const, readability-make-member-function-const)
    void VertexBuffer::SetData(const void* data, std::uint32_t size, BufferUsage usage)
    {
        Bind();
        GL::BufferData(BufferTarget::ArrayBuffer, size, data, usage);
    }
}  // namespace Nebula::OpenGL
