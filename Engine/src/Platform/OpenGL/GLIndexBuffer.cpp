#include "GLIndexBuffer.hpp"

#include "GLAPI.hpp"

namespace Astrelis::OpenGL
{
    GLIndexBuffer::GLIndexBuffer(std::uint32_t count) : GLIndexBuffer(nullptr, count) {}

    GLIndexBuffer::GLIndexBuffer(const std::uint32_t* data, std::uint32_t count) : m_Count(count), m_Id(0)
    {
        GL::GenBuffers(1, &m_Id);
        SetData(data, count);
    }

    GLIndexBuffer::~GLIndexBuffer() { GL::DeleteBuffers(1, &m_Id); }

    GLIndexBuffer::GLIndexBuffer(GLIndexBuffer&& other) noexcept : m_Count(other.m_Count), m_Id(other.m_Id)
    {
        other.m_Id    = 0;
        other.m_Count = 0;
    }

    GLIndexBuffer& GLIndexBuffer::operator=(GLIndexBuffer&& other) noexcept
    {
        if (this != &other)
        {
            m_Id       = other.m_Id;
            other.m_Id = 0;
        }
        return *this;
    }

    void GLIndexBuffer::Bind() const { GL::BindBuffer(BufferTarget::ElementArrayBuffer, m_Id); }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void GLIndexBuffer::Unbind() const { GL::BindBuffer(BufferTarget::ElementArrayBuffer, 0); }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-const, readability-make-member-function-const)
    void GLIndexBuffer::SetData(const std::uint32_t* data, std::uint32_t count)
    {
        Bind();
        GL::BufferData(BufferTarget::ElementArrayBuffer, count * sizeof(std::uint32_t), data, BufferUsage::StaticDraw);
        m_Count = count;
    }
} // namespace Astrelis::OpenGL
