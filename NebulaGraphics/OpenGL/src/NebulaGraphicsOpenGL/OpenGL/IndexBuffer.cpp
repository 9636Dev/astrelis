#include "IndexBuffer.hpp"

#include "Enum.hpp"

namespace Nebula::OpenGL
{
    IndexBuffer::IndexBuffer() : m_Buffer(0) { GL::GenBuffers(1, &m_Buffer); }

    IndexBuffer::~IndexBuffer() { GL::DeleteBuffers(1, &m_Buffer); }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    void IndexBuffer::SetData(const void* data, unsigned int size, BufferUsage usage)
    {
        Bind();
        GL::BufferData(BufferTarget::ElementArrayBuffer, size, data, usage);
        Unbind();
    }

    void IndexBuffer::Bind() const { GL::BindBuffer(BufferTarget::ElementArrayBuffer, m_Buffer); }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void IndexBuffer::Unbind() const { GL::BindBuffer(BufferTarget::ElementArrayBuffer, 0); }
} // namespace Nebula::OpenGL
