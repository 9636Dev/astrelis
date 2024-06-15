#pragma once

#include <cstdint>

#include "NebulaEngine/Renderer/VertexBuffer.hpp"

namespace Nebula::OpenGL
{
    class GLVertexBuffer : public VertexBuffer
    {
    public:
        explicit GLVertexBuffer(std::uint32_t size);
        GLVertexBuffer(const void* data, std::uint32_t size);
        ~GLVertexBuffer() override;
        GLVertexBuffer(const GLVertexBuffer&)            = delete;
        GLVertexBuffer& operator=(const GLVertexBuffer&) = delete;
        GLVertexBuffer(GLVertexBuffer&&) noexcept;
        GLVertexBuffer& operator=(GLVertexBuffer&&) noexcept;

        void Bind() const override;
        void Unbind() const override;

        void SetData(const void* data, std::uint32_t size) override;

        const VertexBufferLayout& GetLayout() const override { return m_Layout; }
        void SetLayout(const VertexBufferLayout& layout) override { m_Layout = layout; }
    private:
        std::uint32_t m_Id;
        VertexBufferLayout m_Layout;
    };
} // namespace Nebula::OpenGL

