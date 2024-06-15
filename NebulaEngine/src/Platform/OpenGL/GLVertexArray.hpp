#pragma once

#include "NebulaEngine/Renderer/VertexArray.hpp"

#include <cstdint>

namespace Nebula::OpenGL
{
    class GLVertexArray : public VertexArray
    {
    public:
        GLVertexArray();
        ~GLVertexArray() override;
        GLVertexArray(const GLVertexArray&) = delete;
        GLVertexArray& operator=(const GLVertexArray&) = delete;
        GLVertexArray(GLVertexArray&& other) noexcept;
        GLVertexArray& operator=(GLVertexArray&& other) noexcept;

        void AddVertexBuffer(const Ptr<VertexBuffer>& vertexBuffer) override;
        void SetIndexBuffer(Ref<IndexBuffer> indexBuffer) override;

        Ref<IndexBuffer>& GetIndexBuffer() override { return m_IndexBuffer; }
        const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

        void Bind() const override;
        void Unbind() const override;
    private:
        std::uint32_t m_Id;
        Ref<IndexBuffer> m_IndexBuffer;
    };
} // namespace Nebula::OpenGL
