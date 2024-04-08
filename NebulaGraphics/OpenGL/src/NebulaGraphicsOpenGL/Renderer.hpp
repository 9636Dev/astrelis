#pragma once

#include <NebulaGraphicsCore/Window.hpp>
#include <NebulaGraphicsCore/Renderer.hpp>

#include "OpenGL/GL.hpp" // TODO: Remove this include
#include "OpenGL/IndexBuffer.hpp"
#include "OpenGL/VertexArray.hpp"
#include "OpenGL/VertexBuffer.hpp"
#include "OpenGL/Shader.hpp"

namespace Nebula
{
    class OpenGLRenderer : public Renderer
    {
    public:
        OpenGLRenderer();
        ~OpenGLRenderer() override;
        OpenGLRenderer(const OpenGLRenderer&) = delete;
        OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
        OpenGLRenderer(OpenGLRenderer&&) = delete;
        OpenGLRenderer& operator=(OpenGLRenderer&&) = delete;

        void Render() override;

    private:
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        inline static std::size_t s_RendererCount = 0;

        OpenGL::VertexArray vao;
        OpenGL::VertexBuffer vbo;
        OpenGL::IndexBuffer ibo;
        OpenGL::Shader vertexShader;
        OpenGL::Shader fragmentShader;
        OpenGL::ShaderProgram shaderProgram;
    };
} // namespace Nebula
