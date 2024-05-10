#pragma once

#include "Enum.hpp"
#include "GL.hpp"

namespace Nebula::OpenGL
{
    class UniformBuffer
    {
    public:
        UniformBuffer();
        ~UniformBuffer();
        UniformBuffer(const UniformBuffer&)            = delete;
        UniformBuffer& operator=(const UniformBuffer&) = delete;

        UniformBuffer(UniformBuffer&& other) noexcept : m_Buffer(other.m_Buffer) { other.m_Buffer = 0; }

        UniformBuffer& operator=(UniformBuffer&& other) noexcept
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

        void BindBase(std::uint32_t index) const;
        void Bind() const;
        void Unbind() const;
    private:
        Buffer_t m_Buffer;
    };
} // namespace Nebula::OpenGL
