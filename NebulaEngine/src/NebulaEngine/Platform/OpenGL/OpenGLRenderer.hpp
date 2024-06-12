#pragma once

#include "NebulaEngine/Platform/OpenGL/API/Shader.hpp"
#include "NebulaEngine/Platform/OpenGL/API/VertexBuffer.hpp"
#include "NebulaEngine/Renderer/Renderer.hpp"
#include "NebulaEngine/Renderer/Vertex.hpp"

#include "API/IndexBuffer.hpp"
#include "API/VertexArray.hpp"

#include <vector>

namespace Nebula
{
    class OpenGLRenderer : public Renderer
    {
    public:
        OpenGLRenderer(Ref<Window> window, Bounds bounds);
        ~OpenGLRenderer() override;
        OpenGLRenderer(const OpenGLRenderer&)            = delete;
        OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
        OpenGLRenderer(OpenGLRenderer&&)                 = delete;
        OpenGLRenderer& operator=(OpenGLRenderer&&)      = delete;

        void Viewport(const Bounds& bounds) override;
        void Clear() override;

        void BeginFrame() override;
        void EndFrame() override;
        void DrawMesh(const StaticMesh& mesh, const Transform& transform) override;
        void InstanceMesh(const StaticMesh& mesh, std::vector<Transform> transforms) override;

        static Result<Ptr<OpenGLRenderer>, std::string> Create(Ref<Window> window, Bounds bounds);
    private:
        OpenGL::VertexArray m_BatchVertexArray;
        OpenGL::VertexBuffer m_BatchVertexBuffer;
        OpenGL::IndexBuffer m_BatchIndexBuffer;
        OpenGL::Program m_BatchProgram;

        OpenGL::VertexArray m_InstanceVertexArray;
        OpenGL::VertexBuffer m_InstanceVertexBuffer;
        OpenGL::IndexBuffer m_InstanceIndexBuffer;
        OpenGL::VertexBuffer m_InstanceInstanceBuffer;
        OpenGL::Program m_InstanceProgram;

        void DrawBatch();

        constexpr static std::size_t s_BatchSize = 64;
        std::vector<Vertex> m_Vertices;
        std::vector<Matrix4f> m_InstanceData;
        std::vector<std::uint32_t> m_Indices;

        Bounds m_Bounds;
    };
} // namespace Nebula
