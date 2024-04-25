#pragma once

#include "OpenGL/IndexBuffer.hpp"
#include "OpenGL/Shader.hpp"
#include "OpenGL/UniformBuffer.hpp"
#include "OpenGL/VertexArray.hpp"
#include "OpenGL/VertexBuffer.hpp"
#include "Window.hpp"

#include "NebulaGraphicsCore/Renderer.hpp"
#include "NebulaGraphicsCore/Window.hpp"

namespace Nebula
{
    // This is only temporary, we will have a more complex renderer later
    struct GLRenderableObject
    {
        OpenGL::VertexArray VertexArray;
        OpenGL::VertexBuffer VertexBuffer;
        OpenGL::IndexBuffer IndexBuffer;
    };

    struct GLRenderPassObject
    {
        OpenGL::ShaderProgram ShaderProgram;
        OpenGL::UniformBuffer UniformBuffer;
    };

    class OpenGLRenderer : public Renderer
    {
    public:
        explicit OpenGLRenderer(std::shared_ptr<GLFWWindow> window);
        ~OpenGLRenderer() override;
        OpenGLRenderer(const OpenGLRenderer&)            = delete;
        OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
        OpenGLRenderer(OpenGLRenderer&&)                 = delete;
        OpenGLRenderer& operator=(OpenGLRenderer&&)      = delete;

        void Render() override;

        void InternalAddRenderPass(RenderPass renderPass, std::size_t insertionIndex) override;
        void InternalRemoveRenderPass(std::size_t index) override;
        void InternalAddRenderableObject(RenderableObject renderableObject, std::size_t renderPassIndex) override;
        void InternalRemoveRenderableObject(std::size_t index) override;

        void OnResize(std::uint32_t width, std::uint32_t height) override;
        void SetClearColor(float red, float green, float blue, float alpha) override;
    private:
        std::vector<GLRenderableObject> m_GLRenderableObjects;
        std::vector<GLRenderPassObject> m_GLRenderPasses;

        std::shared_ptr<GLFWWindow> m_Window;

        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        inline static std::size_t s_RendererCount = 0;
    };
} // namespace Nebula
