#include "IndexBuffer.hpp"
#include "NebulaEngine/Platform/OpenGL/API/GL.hpp"

namespace Nebula::OpenGL
{
    IndexBuffer::IndexBuffer() : m_Id(0)
    {
        GL::GenBuffers(1, &m_Id);
    }

    IndexBuffer::~IndexBuffer()
    {
        GL::DeleteBuffers(1, &m_Id);
    }

    IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept : m_Id(other.m_Id)
    {
        other.m_Id = 0;
    }

    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
    {
        if (this != &other)
        {
            m_Id = other.m_Id;
            other.m_Id = 0;
        }
        return *this;
    }

    void IndexBuffer::Bind() const
    {
        GL::BindBuffer(BufferTarget::ElementArrayBuffer, m_Id);
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void IndexBuffer::Unbind() const
    {
        GL::BindBuffer(BufferTarget::ElementArrayBuffer, 0);
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-const, readability-make-member-function-const)
    void IndexBuffer::SetData(std::uint32_t* data, std::uint32_t count, BufferUsage usage)
    {
        Bind();
        GL::BufferData(BufferTarget::ElementArrayBuffer, count * sizeof(std::uint32_t), data, usage);
    }
} // namespace Nebula::OpenGL
