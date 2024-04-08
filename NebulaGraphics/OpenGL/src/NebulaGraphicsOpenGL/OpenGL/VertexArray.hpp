#pragma once

#include "GL.hpp"

namespace Nebula::OpenGL
{
    class VertexBuffer;
    class VertexBufferLayout;

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();
        VertexArray(const VertexArray&)           = delete;
        VertexArray operator=(const VertexArray&) = delete;

        VertexArray(VertexArray&& other) noexcept : m_VertexArray(other.m_VertexArray) { other.m_VertexArray = 0; }

        VertexArray& operator=(VertexArray&& other) noexcept
        {
            if (this != &other)
            {
                m_VertexArray       = other.m_VertexArray;
                other.m_VertexArray = 0;
            }
            return *this;
        }

        void Bind() const;
        void Unbind() const;

        void AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
    private:
        VertexArray_t m_VertexArray;
    };
} // namespace Nebula::OpenGL
