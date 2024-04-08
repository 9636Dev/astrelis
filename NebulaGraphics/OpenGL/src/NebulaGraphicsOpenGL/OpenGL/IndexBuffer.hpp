#pragma once

#include "GL.hpp"

namespace Nebula::OpenGL
{
    class IndexBuffer
    {
    public:
        IndexBuffer();
        ~IndexBuffer();
        IndexBuffer(const IndexBuffer&)            = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        IndexBuffer(IndexBuffer&& other) noexcept : m_Buffer(other.m_Buffer) { other.m_Buffer = 0; }

        IndexBuffer& operator=(IndexBuffer&& other) noexcept
        {
            if (this != &other)
            {
                m_Buffer       = other.m_Buffer;
                other.m_Buffer = 0;
            }
            return *this;
        }

        void SetData(const void* data, unsigned int size, BufferUsage = BufferUsage::StaticDraw);

        void Bind() const;
        void Unbind() const;
    private:
        Buffer_t m_Buffer;
    };
} // namespace Nebula::OpenGL
