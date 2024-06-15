#include "GLVertexArray.hpp"

#include <cstddef>

#include "GLAPI.hpp"
#include "NebulaEngine/Renderer/VertexBuffer.hpp"
#include "Platform/OpenGL/GLEnum.hpp"

namespace Nebula::OpenGL
{
    GLVertexArray::GLVertexArray() : m_Id(0), m_IndexBuffer(nullptr) { GL::GenVertexArrays(1, &m_Id); }

    GLVertexArray::~GLVertexArray() { GL::DeleteVertexArrays(1, &m_Id); }

    GLVertexArray::GLVertexArray(GLVertexArray&& other) noexcept : m_Id(other.m_Id), m_IndexBuffer(std::move(other.m_IndexBuffer)) { other.m_Id = 0; }

    GLVertexArray& GLVertexArray::operator=(GLVertexArray&& other) noexcept
    {
        m_IndexBuffer = nullptr;
        if (this != &other)
        {
            m_Id       = other.m_Id;
            other.m_Id = 0;
        }
        return *this;
    }

    static GLType GetGLType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
            return GLType::Float;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            NEBULA_CORE_ASSERT(false, "Not implemented yet!");
            return GLType::Float;
        case ShaderDataType::Bool:
            NEBULA_CORE_ASSERT(false, "Not implemented yet!");
            return GLType::Float;
        default:
            NEBULA_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return GLType::Float;
        }
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-const, readability-make-member-function-const)
    void GLVertexArray::AddVertexBuffer(const Ptr<VertexBuffer>& vertexBuffer)
    {
        Bind();
        vertexBuffer->Bind();

        const auto& layout = vertexBuffer->GetLayout();
        std::uint32_t index = layout.GetStartIndex();
        for (const auto& element : layout)
        {
            GL::EnableVertexAttribArray(index);
            // NOLINTNEXTLINE(performance-no-int-to-ptr)
            const void* offset = reinterpret_cast<const void*>(std::uintptr_t(element.Offset));
            GL::VertexAttribPointer(index, ShaderDataTypeHelper::GetCount(element.Type), GetGLType(element.Type), element.Normalized, layout.GetStride(), offset);
            index++;
        }
    }

    void GLVertexArray::SetIndexBuffer(Ref<IndexBuffer> indexBuffer)
    {
        Bind();
        indexBuffer->Bind();
        m_IndexBuffer = std::move(indexBuffer);
    }

    void GLVertexArray::Bind() const { GL::BindVertexArray(m_Id); }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void GLVertexArray::Unbind() const { GL::BindVertexArray(0); }
} // namespace Nebula::OpenGL
