#pragma once

#include "NebulaGraphicsCore/Basic/Renderer.hpp"

#include "NebulaGraphicsOpenGL/OpenGL/IndexBuffer.hpp"
#include "NebulaGraphicsOpenGL/OpenGL/Shader.hpp"
#include "NebulaGraphicsOpenGL/OpenGL/VertexArray.hpp"
#include "NebulaGraphicsOpenGL/OpenGL/VertexBuffer.hpp"

#include "Window.hpp"

namespace Nebula::Basic
{
    namespace GL = Nebula::OpenGL;
    class GLRenderer : public Renderer
    {
    public:
        struct RenderObject
        {
            std::shared_ptr<Mesh> Mesh;
            VisualProps VisualProps;
            Transform Transform;

            GL::VertexArray VertexArray;
            GL::VertexBuffer VertexBuffer;
            GL::IndexBuffer IndexBuffer;


            RenderObject() = default;
        };

        explicit GLRenderer(std::shared_ptr<GLFWWindow> window);
        ~GLRenderer() override;
        GLRenderer(const GLRenderer&) = delete;
        GLRenderer& operator=(const GLRenderer&) = delete;
        GLRenderer(GLRenderer&&) = delete;
        GLRenderer& operator=(GLRenderer&&) = delete;

        void AddMesh(std::shared_ptr<Mesh> mesh, const VisualProps& visualProps, const Transform& transform) override;
        void AddMeshInstanced(std::shared_ptr<Mesh> mesh, const VisualProps& visualProps, const std::vector<Transform>& transforms) override;
        void SetClearColor(float red, float green, float blue, float alpha) override;
        void Render() override;

    private:
        GL::ShaderProgram m_ShaderProgram;
        std::shared_ptr<GLFWWindow> m_Window;
        std::vector<RenderObject> m_RenderObjects;
    };
}  // namespace Nebula::Basic
