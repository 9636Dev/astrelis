#include "VertexBuffer.hpp"

namespace Nebula::OpenGL
{
    VertexBufferLayout::VertexBufferLayout() : m_Stride(0), m_CurrentAttributeIndex(0) {}

    VertexBufferLayout::~VertexBufferLayout() = default;

    Buffer_t GenerateVertexBuffer()
    {
        Buffer_t buffer = 0;
        GL::GenBuffers(1, &buffer);
        return buffer;
    }

    VertexBuffer::VertexBuffer() : m_Buffer(GenerateVertexBuffer()) {}

    VertexBuffer::~VertexBuffer() { GL::DeleteBuffers(1, &m_Buffer); }

    void VertexBuffer::SetData(const void* data, std::uint32_t size, BufferUsage usage)
    {
        Bind();
        GL::BufferData(BufferTarget::ArrayBuffer, size, data, usage);
        Unbind();
    }

    void VertexBuffer::SetSubData(const void* data, std::uint32_t size, std::uint32_t offset)
    {
        Bind();
        GL::BufferSubData(BufferTarget::ArrayBuffer, offset, size, data);
    }

    void VertexBuffer::Bind() const { GL::BindBuffer(BufferTarget::ArrayBuffer, m_Buffer); }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void VertexBuffer::Unbind() const { GL::BindBuffer(BufferTarget::ArrayBuffer, 0); }

    void VertexBuffer::PrintDebugInfo() const {}
} // namespace Nebula::OpenGL
