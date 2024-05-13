#include "BasicRenderer.hpp"
#include "NebulaCore/Log.hpp"
#include "NebulaGraphicsCore/Basic/Renderer.hpp"
#include "NebulaGraphicsOpenGL/Renderer.hpp"

#include "Core.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Nebula::Basic
{

    GLRenderer::GLRenderer(std::shared_ptr<GLFWWindow> window) : m_Window(std::move(window))
    {
        Nebula::OpenGLRenderer::s_RendererCount++;

        // We set the framebuffer size callback
        m_Window->SetFrameBufferSizeCallback(
            [](std::int32_t width, std::int32_t height) { glViewport(0, 0, width, height); });

        // We need shaders
        Nebula::OpenGL::Shader vertexShader(Nebula::OpenGL::ShaderType::VertexShader);
        vertexShader.ShaderSource(R"(
            #version 410

            uniform mat4 u_Model;
            uniform vec4 u_Color;

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec2 a_TexCoord;

            layout(location = 0) out vec2 v_TexCoord;
            layout(location = 1) out vec4 v_Color;

            void main()
            {
                // We are using row-major matrices
                gl_Position = vec4(a_Position, 1.0) * u_Model;
                v_TexCoord = a_TexCoord;
                v_Color = u_Color;
            }
        )");

        if (!vertexShader.Compile())
        {
            NEB_LOG_FATAL("Failed to compile vertex shader: {}", vertexShader.GetInfoLog());
            return;
        }

        Nebula::OpenGL::Shader fragmentShader(Nebula::OpenGL::ShaderType::FragmentShader);
        fragmentShader.ShaderSource(R"(
            #version 410

            layout(location = 0) in vec2 v_TexCoord;
            layout(location = 1) in vec4 v_Color;

            layout(location = 0) out vec4 color;

            void main()
            {
                color = v_Color;
            }
        )");

        if (!fragmentShader.Compile())
        {
            NEB_LOG_FATAL("Failed to compile fragment shader: {}", fragmentShader.GetInfoLog());
            return;
        }

        m_ShaderProgram.AttachShader(vertexShader);
        m_ShaderProgram.AttachShader(fragmentShader);

        if (!m_ShaderProgram.Link())
        {
            NEB_LOG_FATAL("Failed to link shader program: {}", m_ShaderProgram.GetInfoLog());
            return;
        }
    }

    GLRenderer::~GLRenderer()
    {
        m_Window->SetFrameBufferSizeCallback([](std::int32_t, std::int32_t) {});

        if (--Nebula::OpenGLRenderer::s_RendererCount == 0)
        {
            Nebula::OpenGL::GL::Terminate();
        }
    }

    void GLRenderer::AddMesh(std::shared_ptr<Mesh> mesh, const VisualProps& visualProps, const Transform& transform)
    {
        m_RenderObjects.emplace_back();
        auto& renderObject = m_RenderObjects.back();
        renderObject.Mesh = std::move(mesh);
        renderObject.VisualProps = visualProps;
        renderObject.Transform = transform;

        renderObject.VertexArray = Nebula::OpenGL::VertexArray();
        renderObject.VertexBuffer = Nebula::OpenGL::VertexBuffer();
        renderObject.IndexBuffer = Nebula::OpenGL::IndexBuffer();
        GL::VertexBufferLayout layout;
        layout.Push<float>(3);  // Position
        layout.Push<float>(2);  // Texture coordinates

        renderObject.VertexArray.AddBuffer(renderObject.VertexBuffer, layout);

        renderObject.VertexBuffer.SetData(renderObject.Mesh->GetVertices().data(), renderObject.Mesh->GetVertices().size() * sizeof(Vertex));
        renderObject.IndexBuffer.SetData(renderObject.Mesh->GetIndices().data(), renderObject.Mesh->GetIndices().size() * sizeof(std::uint32_t));

    }

    void GLRenderer::AddMeshInstanced(std::shared_ptr<Mesh> mesh, const VisualProps& visualProps, const std::vector<Transform>& transforms)
    {
    }

    void GLRenderer::Render()
    {
        GL::GL::Clear(GL::ClearTarget::ColorBufferBit | GL::ClearTarget::DepthBufferBit);

        m_ShaderProgram.Use();
        for (const auto& renderObject : m_RenderObjects)
        {
            auto model = glm::identity<glm::mat4>();
            // Copy the data from Eigen to glm
            auto modelMatrix = renderObject.Transform.GetModelMatrix();
            model = glm::make_mat4(modelMatrix.data());
            NEB_CORE_LOG_TRACE("Model matrix: {0}", glm::to_string(model));

            m_ShaderProgram.SetUniform("u_Model", model);
            glm::vec4 color(renderObject.VisualProps.Color.x(), renderObject.VisualProps.Color.y(), renderObject.VisualProps.Color.z(), renderObject.VisualProps.Color.w());
            m_ShaderProgram.SetUniform("u_Color", color);

            renderObject.VertexArray.Bind();
            renderObject.IndexBuffer.Bind();
             GL::GL::DrawElements(GL::DrawMode::Triangles, static_cast<std::int32_t>(renderObject.Mesh->GetIndices().size()), GL::Type::UnsignedInt, nullptr);
        }
    }

    void GLRenderer::SetClearColor(float red, float green, float blue, float alpha)
    {
        GL::GL::ClearColor(red, green, blue, alpha);
    }

}  // namespace Nebula::Basic

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"

extern "C" NEBULA_GRAPHICS_OPENGL_API Nebula::Basic::RendererCreationResult
    nebulaGraphicsOpenGLCreateBasicOpenGLRenderer(const Nebula::OpenGLContext& context,
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

    auto renderer = std::make_shared<Nebula::Basic::GLRenderer>(windowPtr);
    return {renderer, Nebula::RendererCreationResult::ErrorType::None};
}

#pragma clang diagnostic pop
