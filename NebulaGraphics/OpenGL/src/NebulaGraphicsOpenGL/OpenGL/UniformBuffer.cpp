#include "UniformBuffer.hpp"

namespace Nebula::OpenGL
{
    Buffer_t GenerateUniformBuffer()
    {
        Buffer_t buffer = 0;
        GL::GenBuffers(1, &buffer);
        return buffer;
    }
    UniformBuffer::UniformBuffer() : m_Buffer(GenerateUniformBuffer()) {}

    UniformBuffer::~UniformBuffer() { GL::DeleteBuffers(1, &m_Buffer); }

    void UniformBuffer::SetData(const void* data, std::uint32_t size, BufferUsage usage)
    {
        Bind();
        GL::BufferData(BufferTarget::UniformBuffer, size, data, usage);
        Unbind();
    }

    void UniformBuffer::SetSubData(const void* data, std::uint32_t size, std::uint32_t offset)
    {
        Bind();
        GL::BufferSubData(BufferTarget::UniformBuffer, offset, size, data);
    }

    void UniformBuffer::BindBase(std::uint32_t index) const
    {
        GL::BindBufferBase(BufferTarget::UniformBuffer, index, m_Buffer);
    }

    void UniformBuffer::Bind() const { GL::BindBuffer(BufferTarget::UniformBuffer, m_Buffer); }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void UniformBuffer::Unbind() const { GL::BindBuffer(BufferTarget::UniformBuffer, 0); }
}  // namespace Nebula::OpenGL
