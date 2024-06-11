#include "VertexArray.hpp"

#include <cstddef>

#include "NebulaEngine/Platform/OpenGL/API/GL.hpp"

#include "VertexBuffer.hpp"

namespace Nebula::OpenGL
{
    VertexArray::VertexArray() : m_Id(0) { GL::GenVertexArrays(1, &m_Id); }

    VertexArray::~VertexArray() { GL::DeleteVertexArrays(1, &m_Id); }

    VertexArray::VertexArray(VertexArray&& other) noexcept : m_Id(other.m_Id) { other.m_Id = 0; }

    VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
    {
        if (this != &other)
        {
            m_Id       = other.m_Id;
            other.m_Id = 0;
        }
        return *this;
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-const, readability-make-member-function-const)
    void VertexArray::AddVertexBuffer(const VertexBuffer& vertexBuffer, const VertexBufferFormat& format)
    {
        Bind();
        vertexBuffer.Bind();

        const auto& attributes = format.m_Attributes;
        std::size_t offset     = 0;
        for (std::size_t i = format.m_StartIndex; i < attributes.size() + format.m_StartIndex; ++i)
        {
            const auto& attribute = attributes[i];
            // NOLINTNEXTLINE(performance-no-int-to-ptr)
            const void* ptr       = reinterpret_cast<const void*>(offset);
            GL::EnableVertexAttribArray(i);
            GL::VertexAttribPointer(i, attribute.size, attribute.type, attribute.normalized, format.m_Stride, ptr);
            if (attribute.instanced)
            {
                GL::VertexAttribDivisor(i, attribute.divisor);
            }
            offset += static_cast<std::size_t>(attribute.size * GetGLTypeSize(attribute.type));
        }
    }

    void VertexArray::Bind() const { GL::BindVertexArray(m_Id); }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void VertexArray::Unbind() const { GL::BindVertexArray(0); }
} // namespace Nebula::OpenGL
