#include "GLVertexBuffer.hpp"

#include "GLAPI.hpp"

namespace Astrelis::OpenGL
{
    GLVertexBuffer::GLVertexBuffer(std::uint32_t size) : GLVertexBuffer(nullptr, size)
    {
        // We seperate here since we are not sure if all rendering APIs can be passed 'nullptr' as data
    }

    GLVertexBuffer::GLVertexBuffer(const void* data, std::uint32_t size) : m_Id(0), m_Layout({})
    {
        GL::GenBuffers(1, &m_Id);
        Bind();
        SetData(data, size);
    }

    GLVertexBuffer::~GLVertexBuffer() { GL::DeleteBuffers(1, &m_Id); }

    GLVertexBuffer::GLVertexBuffer(GLVertexBuffer&& other) noexcept : m_Id(other.m_Id), m_Layout(std::move(other.m_Layout)) { other.m_Id = 0; }

    GLVertexBuffer& GLVertexBuffer::operator=(GLVertexBuffer&& other) noexcept
    {
        if (this != &other)
        {
            GL::DeleteBuffers(1, &m_Id);
            m_Id       = other.m_Id;
            other.m_Id = 0;
        }
        return *this;
    }

    void GLVertexBuffer::Bind() const { GL::BindBuffer(BufferTarget::ArrayBuffer, m_Id); }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void GLVertexBuffer::Unbind() const { GL::BindBuffer(BufferTarget::ArrayBuffer, 0); }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-const, readability-make-member-function-const)
    void GLVertexBuffer::SetData(const void* data, std::uint32_t size)
    {
        Bind();
        GL::BufferData(BufferTarget::ArrayBuffer, size, data, BufferUsage::StaticDraw);
    }
} // namespace Astrelis::OpenGL
