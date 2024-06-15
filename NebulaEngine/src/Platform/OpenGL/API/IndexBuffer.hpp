#pragma once

#include "NebulaEngine/Platform/OpenGL/API/Enum.hpp"
#include <cstdint>
namespace Nebula::OpenGL
{
    class IndexBuffer
    {
    public:
        IndexBuffer();
        ~IndexBuffer();
        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&& other) noexcept;
        IndexBuffer& operator=(IndexBuffer&& other) noexcept;

        void Bind() const;
        void Unbind() const;

        void SetData(const std::uint32_t* data, std::uint32_t count, BufferUsage usage);
    private:
        std::uint32_t m_Id;
    };
} // namespace Nebula::OpenGL
