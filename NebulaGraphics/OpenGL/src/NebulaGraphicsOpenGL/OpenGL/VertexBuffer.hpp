#pragma once

#include "Enum.hpp"
#include "GL.hpp"

#include <vector>

namespace Nebula::OpenGL
{
    class VertexArray;
    class GLMeshBackend2D;

    class VertexBufferLayout
    {
    public:
        friend class VertexArray;

        struct Attribute
        {
            std::uint16_t Size;
            Type Type;
            bool Normalized;
            bool Instanced;
            // -1 means that the strjde is calculated automatically
            std::int32_t Stride;
            // -1 means that the offset is calculated automatically
            sizeiptr_t Offset;

            Attribute(std::uint16_t size,
                      OpenGL::Type type,
                      bool normalized,
                      bool instanced      = false,
                      std::int32_t stride = -1,
                      sizeiptr_t offset   = -1) :
                Size(size),
                Type(type),
                Normalized(normalized),
                Instanced(instanced),
                Stride(stride),
                Offset(offset)
            {
            }
        };

        VertexBufferLayout();
        ~VertexBufferLayout();
        VertexBufferLayout(const VertexBufferLayout&)            = default;
        VertexBufferLayout& operator=(const VertexBufferLayout&) = default;
        VertexBufferLayout(VertexBufferLayout&&)                 = delete;
        VertexBufferLayout& operator=(VertexBufferLayout&&)      = delete;

        inline void
            PushMat4(bool normalized = false, bool instanced = false, std::int32_t stride = -1, sizeiptr_t offset = -1)
        {
            Push<float>(4, normalized, instanced, stride, offset);
            Push<float>(4, normalized, instanced, stride, offset);
            Push<float>(4, normalized, instanced, stride, offset);
            Push<float>(4, normalized, instanced, stride, offset);
        }

        /// <summary>
        /// Cannot be mixed with 'push' functions
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="index"></param>
        /// <param name="size"></param>
        /// <param name="normalized"></param>
        /// <param name="instanced"></param>
        /// <param name="stride"></param>
        /// <param name="offset"></param>
        template<GLType T>
        inline void SetAttribute(std::uint32_t index,
                                 std::uint16_t size,
                                 bool normalized     = false,
                                 bool instanced      = false,
                                 std::int32_t stride = -1,
                                 sizeiptr_t offset   = -1)
        {
            m_Attributes.emplace_back(index,
                                      Attribute(size, GL::GetGLType<T>(), normalized, instanced, stride, offset));
            m_Stride += size * sizeof(T);
        }

        template<GLType T>
        inline void Push(std::uint16_t size,
                         bool normalized     = false,
                         bool instanced      = false,
                         std::int32_t stride = -1,
                         sizeiptr_t offset   = -1)
        {
            std::pair<std::uint32_t, Attribute> attribute(
                m_CurrentAttributeIndex++, Attribute(size, GL::GetGLType<T>(), normalized, instanced, stride, offset));
            m_Attributes.push_back(attribute);
            m_Stride += size * sizeof(T);
        }

        inline void Clear()
        {
            m_Attributes.clear();
            m_CurrentAttributeIndex = 0;
            m_Stride                = 0;
        }

        [[nodiscard]] inline std::uint32_t GetStride() const { return m_Stride; }
    private:
        std::vector<std::pair<std::uint32_t, Attribute>> m_Attributes;
        std::uint32_t m_CurrentAttributeIndex;
        std::uint32_t m_Stride;
    };

    class VertexBuffer
    {
    public:
        friend class VertexArray;
        VertexBuffer();
        ~VertexBuffer();
        VertexBuffer(const VertexBuffer&)            = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        VertexBuffer(VertexBuffer&& other) noexcept : m_Buffer(other.m_Buffer) { other.m_Buffer = 0; }

        VertexBuffer& operator=(VertexBuffer&& other) noexcept
        {
            if (this != &other)
            {
                m_Buffer       = other.m_Buffer;
                other.m_Buffer = 0;
            }
            return *this;
        }

        void SetData(const void* data, std::uint32_t size, BufferUsage = BufferUsage::StaticDraw);
        void SetSubData(const void* data, std::uint32_t size, std::uint32_t offset);
    private:
        void Bind() const;
        void Unbind() const;
        void PrintDebugInfo() const;

        Buffer_t m_Buffer;
    };
} // namespace Nebula::OpenGL
