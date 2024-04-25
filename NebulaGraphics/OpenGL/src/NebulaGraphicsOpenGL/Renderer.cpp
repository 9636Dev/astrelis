#include "Renderer.hpp"

#include "NebulaGraphicsOpenGL/Core.hpp"
#include "NebulaShaderConductor/Conductor.hpp"
#include "OpenGL/GL.hpp"
#include "Window.hpp"

namespace Nebula
{
    OpenGLRenderer::OpenGLRenderer(std::shared_ptr<GLFWWindow> window)
        : m_Window(std::move(window))
    {

        s_RendererCount++;

        // We set the framebuffer size callback
        m_Window->SetFrameBufferSizeCallback([](std::int32_t width, std::int32_t height) {
            glViewport(0, 0, width, height);
        });
    }

    OpenGLRenderer::~OpenGLRenderer()
    {
        m_Window->SetFrameBufferSizeCallback([](std::int32_t, std::int32_t) {});

        if (--s_RendererCount == 0)
        {
            OpenGL::GL::Terminate();
        }

    }

    void OpenGLRenderer::InternalAddRenderPass(RenderPass renderPass, std::size_t insertionIndex)
    {
        ShaderConductor::ShaderConductor shaderConductor;
        // Compile the shader program
        OpenGL::ShaderProgram shaderProgram;
        ShaderConductor::GLSLOutput output;
        output.Version = 410;
        {
            OpenGL::Shader vertexShader(OpenGL::ShaderType::VertexShader);
            auto spirv = shaderConductor.CompileToSPIRV(renderPass.ShaderProgram.VertexShader, output);
            if (!spirv.success)
            {
                throw std::runtime_error(spirv.errorMessage);
            }

            auto glsl = ShaderConductor::ShaderConductor::CompileToGLSL(spirv.spirvCode, output);
            if (!glsl.second.empty())
            {
                throw std::runtime_error(glsl.second);
            }

            vertexShader.ShaderSource(glsl.first);
            if (!vertexShader.Compile())
            {
                throw std::runtime_error(vertexShader.GetInfoLog());
            }

            shaderProgram.AttachShader(vertexShader);
        }

        {
            OpenGL::Shader fragmentShader(OpenGL::ShaderType::FragmentShader);
            auto spirv = shaderConductor.CompileToSPIRV(renderPass.ShaderProgram.FragmentShader, output);
            if (!spirv.success)
            {
                throw std::runtime_error(spirv.errorMessage);
            }

            auto glsl = ShaderConductor::ShaderConductor::CompileToGLSL(spirv.spirvCode, output);
            if (!glsl.second.empty())
            {
                throw std::runtime_error(glsl.second);
            }

            fragmentShader.ShaderSource(glsl.first);
            if (!fragmentShader.Compile())
            {
                throw std::runtime_error(fragmentShader.GetInfoLog());
            }

            shaderProgram.AttachShader(fragmentShader);
        }

        if (!shaderProgram.Link())
        {
            throw std::runtime_error(shaderProgram.GetInfoLog());
        }

        GLRenderPassObject glRenderPassObject;
        glRenderPassObject.ShaderProgram = std::move(shaderProgram);
        glRenderPassObject.UniformBuffer = OpenGL::UniformBuffer();

        m_GLRenderPasses.push_back(std::move(glRenderPassObject));
    }

    void OpenGLRenderer::InternalRemoveRenderPass(std::size_t index)
    {
        m_GLRenderPasses.erase(m_GLRenderPasses.begin() + index);
    }

    void OpenGLRenderer::InternalAddRenderableObject(RenderableObject renderableObject, std::size_t renderPassIndex)
    {
        GLRenderableObject glRenderableObject;
        glRenderableObject.VertexArray = OpenGL::VertexArray();
        glRenderableObject.VertexBuffer = OpenGL::VertexBuffer();
        glRenderableObject.IndexBuffer = OpenGL::IndexBuffer();

        glRenderableObject.VertexBuffer.SetData(renderableObject.m_Mesh->GetVertexData().data(), static_cast<std::uint32_t>(renderableObject.m_Mesh->GetVertexData().size() * sizeof(float)));
        OpenGL::VertexBufferLayout layout;
        layout.Push<float>(2);
        glRenderableObject.VertexArray.AddBuffer(glRenderableObject.VertexBuffer, layout);

        glRenderableObject.IndexBuffer.SetData(renderableObject.m_Mesh->GetIndexData().data(), static_cast<std::uint32_t>(renderableObject.m_Mesh->GetIndexData().size() * sizeof(std::uint32_t)));

        m_GLRenderableObjects.push_back(std::move(glRenderableObject));
    }

    void OpenGLRenderer::InternalRemoveRenderableObject(std::size_t index)
    {
        m_GLRenderableObjects.erase(m_GLRenderableObjects.begin() + index);
    }

    void OpenGLRenderer::Render()
    {
        // We are just using this function to test the renderer at the moment
        NEB_CORE_LOG_DEBUG("Clearing the screen");
        OpenGL::GL::Clear(OpenGL::ClearTarget::ColorBufferBit | OpenGL::ClearTarget::DepthBufferBit);

        NEB_CORE_LOG_DEBUG("Rendering the objects");
        for (std::size_t i = 0; i < m_RenderPasses.size(); i++)
        {
            NEB_CORE_LOG_DEBUG("Rendering objects in render pass {0}", i);
            m_GLRenderPasses[i].ShaderProgram.Use();
            //m_GLRenderPasses[i].UniformBuffer.Bind();
            //m_GLRenderPasses[i].UniformBuffer.BindBase(0);

            for (std::size_t j = 0; j < m_RenderPassObjectCount[i]; j++)
            {
                NEB_CORE_LOG_DEBUG("Rendering object {0}", j);
                //Matrix4f modelMatrix = m_RenderableObjects[j].m_Transform.GetModelMatrix();
                //m_GLRenderPasses[i].UniformBuffer.SetData(&modelMatrix, sizeof(Matrix4f));
                m_GLRenderableObjects[j].VertexArray.Bind();
                m_GLRenderableObjects[j].IndexBuffer.Bind();

                NEB_CORE_LOG_DEBUG("Drawing the elements");
                OpenGL::GL::DrawElements(OpenGL::DrawMode::Triangles, m_RenderableObjects[j].m_Mesh->GetIndexData().size(), OpenGL::GL::GetGLType<std::uint32_t>(), nullptr);
            }
        }
    }


    void OpenGLRenderer::OnResize(std::uint32_t width, std::uint32_t height)
    {
        // We use the frame buffer for OpenGL viewport
    }

    void OpenGLRenderer::SetClearColor(float red, float green, float blue, float alpha)
    {
        OpenGL::GL::ClearColor(red, green, blue, alpha);
    }
} // namespace Nebula
//
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern "C" NEBULA_GRAPHICS_OPENGL_API Nebula::RendererCreationResult
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

    auto renderer = std::make_shared<Nebula::OpenGLRenderer>(windowPtr);
    return {renderer, Nebula::RendererCreationResult::ErrorType::None};
}
#pragma clang diagnostic pop
