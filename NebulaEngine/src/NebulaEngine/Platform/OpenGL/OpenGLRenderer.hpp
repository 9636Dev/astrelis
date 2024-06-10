#pragma once

#include "NebulaEngine/Renderer/Renderer.hpp"
#include "NebulaEngine/Renderer/Vertex.hpp"
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

        void Clear() override;

        void BeginFrame() override;
        void EndFrame() override;
        void DrawMesh(const StaticMesh& mesh, const Transform& transform) override;

        static Result<Ptr<OpenGLRenderer>, std::string> Create(Ref<Window> window, Bounds bounds);

    private:
        std::uint32_t m_VAO;
        std::uint32_t m_VBO;
        std::uint32_t m_IBO;
        std::uint32_t m_Shader;

        void DrawBatch();

        constexpr static std::size_t s_BatchSize = 64;
        std::vector<Vertex> m_Vertices;
        std::vector<std::uint32_t> m_Indices;

        Bounds m_Bounds;
    };
} // namespace Nebula
