#include "Renderer.hpp"

#include "NebulaCore/Log.hpp"
#include "NebulaGraphicsOpenGL/Core.hpp"
#include "NebulaGraphicsOpenGL/OpenGL/VertexBuffer.hpp"
#include "OpenGL/GL.hpp"
#include "Window.hpp"

#include <array>

namespace Nebula
{
    OpenGLRenderer::OpenGLRenderer()
        : vertexShader(OpenGL::ShaderType::VertexShader), fragmentShader(OpenGL::ShaderType::FragmentShader)
    {

        s_RendererCount++;

        OpenGL::GL::ClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        float vertices[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
        };
        vbo.SetData(vertices, sizeof(vertices));

        OpenGL::VertexBufferLayout layout;
        layout.Push<float>(2);
        vao.AddBuffer(vbo, layout);

        std::uint32_t indices[] = {
            0, 1, 2,
        };

        ibo.SetData(indices, sizeof(indices));

        vertexShader.ShaderSource(R"(
            #version 330 core

            layout(location = 0) in vec4 position;

            void main()
            {
                gl_Position = position;
            }
        )");

        fragmentShader.ShaderSource(R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            void main()
            {
                color = vec4(1.0, 0.0, 0.0, 1.0);
            }
        )");

        if (!vertexShader.Compile())
        {
            NEB_CORE_LOG_ERROR("Failed to compile vertex shader");
        }

        if (!fragmentShader.Compile())
        {
            NEB_CORE_LOG_ERROR("Failed to compile fragment shader");
        }

        shaderProgram.AttachShader(vertexShader);
        shaderProgram.AttachShader(fragmentShader);

        if (!shaderProgram.Link())
        {
            NEB_CORE_LOG_ERROR("Failed to link shader program");
        }

        shaderProgram.Use();
        vao.Bind();
        ibo.Bind();

    }

    OpenGLRenderer::~OpenGLRenderer()
    {
        if (--s_RendererCount == 0)
        {
            OpenGL::GL::Terminate();
        }

    }

    void OpenGLRenderer::Render()
    {
        // We are just using this function to test the renderer at the moment
        OpenGL::GL::Clear(OpenGL::ClearTarget::ColorBufferBit);
        vao.Bind();
        ibo.Bind();
        shaderProgram.Use();
        OpenGL::GL::DrawElements(OpenGL::DrawMode::Triangles, 3, OpenGL::Type::UnsignedInt, nullptr);

    }
} // namespace Nebula

extern "C"
{
    NEBULA_GRAPHICS_OPENGL_API Nebula::RendererCreationResult
        nebulaGraphicsOpenGLCreateOpenGLRenderer(const Nebula::OpenGLContext& context,
                                                 const std::shared_ptr<Nebula::Window>& window)
    {
        std::shared_ptr<Nebula::GLFWWindow> windowPtr = std::dynamic_pointer_cast<Nebula::GLFWWindow>(window);
        if (context.MajorVersion == 0 || windowPtr == nullptr)
        {
            return {nullptr, Nebula::RendererCreationResult::ErrorType::InvalidArguments};
        }

        windowPtr->MakeContextCurrent();
        Nebula::OpenGL::GLVersion version = {context.MajorVersion, context.MinorVersion};
        Nebula::OpenGL::GL::Init(version);

        if (version.Major == 0)
        {
            return {nullptr, Nebula::RendererCreationResult::ErrorType::RendererInitFailed};
        }

        auto renderer = std::make_shared<Nebula::OpenGLRenderer>();
        return {renderer, Nebula::RendererCreationResult::ErrorType::None};
    }
}
