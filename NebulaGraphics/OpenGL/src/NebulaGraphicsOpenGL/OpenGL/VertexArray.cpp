#include "VertexArray.hpp"

#include "VertexBuffer.hpp"

#include <algorithm>

namespace Nebula::OpenGL
{
    VertexArray_t CreateVertexArray()
    {
        VertexArray_t vertexArray = 0;
        GL::GenVertexArrays(1, &vertexArray);
        return vertexArray;
    }

    VertexArray::VertexArray() : m_VertexArray(CreateVertexArray()) { Unbind(); }

    VertexArray::~VertexArray() { GL::DeleteVertexArrays(1, &m_VertexArray); }

    void VertexArray::Bind() const { GL::BindVertexArray(m_VertexArray); }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void VertexArray::Unbind() const { GL::BindVertexArray(0); }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    void VertexArray::AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
    {
        Bind();
        vertexBuffer.Bind();

        // Sort attributes by index
        std::vector<std::pair<std::uint32_t, VertexBufferLayout::Attribute>> attributes = layout.m_Attributes;
        std::sort(attributes.begin(), attributes.end(),
                  [](const std::pair<std::uint32_t, VertexBufferLayout::Attribute>& left,
                     const std::pair<std::uint32_t, VertexBufferLayout::Attribute>& right) {
            return left.first < right.first;
        });

        std::ptrdiff_t offset = 0;
        for (const auto& [index, attribute] : attributes)
        {
            std::int32_t stride   = attribute.Stride == -1 ? static_cast<std::int32_t>(layout.m_Stride)
                                                           : static_cast<std::int32_t>(attribute.Stride);
            // Disabled linting for: reinterpret_cast, and number to pointer conversion (OpenGL Specificication)
            const void* offsetPtr = attribute.Offset == -1 ? /* NOLINT */ reinterpret_cast<const void*>(offset)
                                                           : /* NOLINT */ reinterpret_cast<const void*>(attribute.Offset);

            GL::EnableVertexAttribArray(index);
            if (attribute.Type == Type::Double)
            {
                GL::VertexAttribLPointer(index, attribute.Size, attribute.Type, stride, offsetPtr);
            }
            else
            {
                GL::VertexAttribPointer(index, attribute.Size, attribute.Type, attribute.Normalized, stride, offsetPtr);
            }
            if (attribute.Instanced)
            {
                GL::VertexAttribDivisor(index, 1);
            }

            offset += static_cast<std::int64_t>(attribute.Size * GL::GetSizeOfGLType(attribute.Type));
        }
    }
} // namespace Nebula::OpenGL
