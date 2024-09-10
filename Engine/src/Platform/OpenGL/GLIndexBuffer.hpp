#pragma once

#include "Astrelis/Renderer/IndexBuffer.hpp"
#include <cstdint>

namespace Astrelis::OpenGL
{
    class GLIndexBuffer : public IndexBuffer
    {
    public:
        explicit GLIndexBuffer(std::uint32_t count);
        GLIndexBuffer(const std::uint32_t* data, std::uint32_t count);
        ~GLIndexBuffer() override;
        GLIndexBuffer(const GLIndexBuffer&) = delete;
        GLIndexBuffer& operator=(const GLIndexBuffer&) = delete;
        GLIndexBuffer(GLIndexBuffer&& other) noexcept;
        GLIndexBuffer& operator=(GLIndexBuffer&& other) noexcept;

        void Bind() const override;
        void Unbind() const override;

        void SetData(const std::uint32_t* data, std::uint32_t count) override;
        [[nodiscard]] std::uint32_t GetCount() const override { return m_Count; }
    private:
        std::uint32_t m_Count;
        std::uint32_t m_Id;
    };
} // namespace Astrelis::OpenGL
