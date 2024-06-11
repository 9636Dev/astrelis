#pragma once

#include <cstdint>
#include <vector>

#include "NebulaEngine/Core/Utils/Function.hpp"
#include "NebulaEngine/Platform/OpenGL/API/Enum.hpp"

namespace Nebula::OpenGL
{
    class VertexBufferFormat
    {
    public:
        struct VertexAttribute
        {
            uint32_t size;
            GLType type;
            bool normalized;
            bool instanced = false;
            std::uint32_t divisor = 0;
        };

        VertexBufferFormat() = default;

        void SetStartIndex(std::size_t index) { m_StartIndex = index; }

        template<typename T>
        void Push(std::uint32_t count = 1)
        {
            m_Attributes.push_back({ count, GetGLType<T>(), false });
            m_Stride += count * sizeof(T);
        }

        template<typename T>
        void PushInstanced(std::uint32_t count = 1, std::uint32_t divisor = 1)
        {
            m_Attributes.push_back({ count, GetGLType<T>(), false, true, divisor });
            m_Stride += count * sizeof(T);
        }

    private:
        std::vector<VertexAttribute> m_Attributes;
        std::size_t m_Stride = 0;
        std::size_t m_StartIndex = 0;

        friend class VertexArray;
    };

    class VertexBuffer
    {
    public:
        VertexBuffer();
        ~VertexBuffer();
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&) noexcept;
        VertexBuffer& operator=(VertexBuffer&&) noexcept;

        void Bind() const;
        void Unbind() const;

        void SetData(const void* data, std::uint32_t size, BufferUsage usage);

    private:
        std::uint32_t m_Id;
    };
} // namespace Nebula::OpenGL

