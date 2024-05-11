#include "Renderer.hpp"

#include "NebulaCore/Log.hpp"
#include "NebulaGraphicsOpenGL/Core.hpp"
#include "NebulaGraphicsOpenGL/OpenGL/Enum.hpp"
#include "OpenGL/GL.hpp"
#include "Window.hpp"
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef NEBULA_INCLUDE_IMGUI
    #include <backends/imgui_impl_glfw.h>
    #include <backends/imgui_impl_opengl3.h>
    #include <imgui.h>
#endif

namespace Nebula
{
    OpenGLRenderer::OpenGLRenderer(std::shared_ptr<GLFWWindow> window) : m_Window(std::move(window))
    {
        s_RendererCount++;

        // We set the framebuffer size callback
        m_Window->SetFrameBufferSizeCallback(
            [](std::int32_t width, std::int32_t height) { glViewport(0, 0, width, height); });

#ifdef NEBULA_INCLUDE_IMGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330"); // Should never be lower than 330
#endif
    }

    OpenGLRenderer::~OpenGLRenderer()
    {
        m_Window->SetFrameBufferSizeCallback([](std::int32_t, std::int32_t) {});

#ifdef NEBULA_INCLUDE_IMGUI
#endif

        if (--s_RendererCount == 0)
        {
            OpenGL::GL::Terminate();
        }
    }

    void OpenGLRenderer::InternalAddRenderPass(RenderPass renderPass, std::size_t insertionIndex)
    {
        NEB_CORE_LOG_DEBUG("Generating Render Pass: {0}", insertionIndex);
        NEB_ASSERT(m_AssetLoader.m_Shaders.contains(renderPass.ShaderProgram), "Shader program does not exist");
        auto& shader = m_AssetLoader.m_Shaders[renderPass.ShaderProgram];

        GLRenderPassObject glRenderPassObject;

        {
            OpenGL::Shader vertexShader(OpenGL::ShaderType::VertexShader);
            vertexShader.ShaderSource(shader.VertexSource);
            if (!vertexShader.Compile())
            {
                throw std::runtime_error("GLSL Was not compiled: " + vertexShader.GetInfoLog());
            }

            glRenderPassObject.ShaderProgram.AttachShader(vertexShader);
        }

        {
            OpenGL::Shader fragmentShader(OpenGL::ShaderType::FragmentShader);
            fragmentShader.ShaderSource(shader.PixelSource);

            if (!fragmentShader.Compile())
            {
                throw std::runtime_error("GLSL Was not compiled: " + fragmentShader.GetInfoLog());
            }

            glRenderPassObject.ShaderProgram.AttachShader(fragmentShader);
        }

        if (!glRenderPassObject.ShaderProgram.Link())
        {
            throw std::runtime_error(glRenderPassObject.ShaderProgram.GetInfoLog());
        }

        if (shader.Bindings.size() != 1)
        {
            NEB_CORE_LOG_WARN("Uniform buffer count is not 1 (More than one not supported yet)");
            m_GLRenderPasses.insert(m_GLRenderPasses.begin() + static_cast<std::int64_t>(insertionIndex),
                                    std::move(glRenderPassObject));
            return;
        }

        auto& uniform = shader.Bindings[0];

        glRenderPassObject.ShaderProgram.Use();
        glRenderPassObject.UniformBuffer.Bind();

        if (shader.GlslVersion < 420 && !shader.Glsl420PackEnabled)
        {
            std::uint32_t index = glRenderPassObject.ShaderProgram.GetUniformBlockIndex(uniform.Name);
            if (index != OpenGL::GL::InvalidIndex)
            {
                glRenderPassObject.ShaderProgram.UniformBlockBinding(index, 0);
            }
            else
            {
                NEB_CORE_LOG_WARN("Could not find uniform block index for {0}", uniform.Name);
            }
        }

        glRenderPassObject.UniformBuffer.BindBase(uniform.Slot.value_or(0));

        m_GLRenderPasses.insert(m_GLRenderPasses.begin() + static_cast<std::int64_t>(insertionIndex), std::move(glRenderPassObject));
    }

    void OpenGLRenderer::InternalRemoveRenderPass(std::size_t index)
    {
        m_GLRenderPasses.erase(m_GLRenderPasses.begin() + static_cast<std::int64_t>(index));
    }

    void OpenGLRenderer::InternalAddRenderableObject(RenderableObject renderableObject, std::size_t renderableIndex)
    {
        GLRenderableObject glRenderableObject;
        glRenderableObject.VertexArray  = OpenGL::VertexArray();
        glRenderableObject.VertexBuffer = OpenGL::VertexBuffer();
        glRenderableObject.IndexBuffer  = OpenGL::IndexBuffer();

        glRenderableObject.VertexBuffer.SetData(
            renderableObject.m_Mesh->GetVertexData().data(),
            static_cast<std::uint32_t>(renderableObject.m_Mesh->GetVertexData().size() * sizeof(float)));
        OpenGL::VertexBufferLayout layout;
        layout.Push<float>(2);
        glRenderableObject.VertexArray.AddBuffer(glRenderableObject.VertexBuffer, layout);

        glRenderableObject.IndexBuffer.SetData(
            renderableObject.m_Mesh->GetIndexData().data(),
            static_cast<std::uint32_t>(renderableObject.m_Mesh->GetIndexData().size() * sizeof(std::uint32_t)));

        m_GLRenderableObjects.insert(m_GLRenderableObjects.begin() + static_cast<std::int64_t>(renderableIndex),
                                     std::move(glRenderableObject));
    }

    void OpenGLRenderer::InternalRemoveRenderableObject(std::size_t index)
    {
        m_GLRenderableObjects.erase(m_GLRenderableObjects.begin() + static_cast<std::int64_t>(index));
    }

    void OpenGLRenderer::Render()
    {
        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        // We are just using this function to test the renderer at the moment
        OpenGL::GL::Clear(OpenGL::ClearTarget::ColorBufferBit | OpenGL::ClearTarget::DepthBufferBit);

        std::size_t index = 0;
        for (std::size_t i = 0; i < m_RenderPasses.size(); i++)
        {
            std::size_t end = index + m_RenderPassObjectCount[i];
            for (std::size_t j = index; j < end; j++)
            {
                // TODO(9636D): Logid bug here, we are not using the correct index (Unless we are using the first render pass)
                Matrix4f modelMatrix = m_RenderableObjects[j].m_Transform.GetModelMatrix();

                // TODO(Dev9636): Actually parse the Binding and set the data
                m_GLRenderPasses[i].UniformBuffer.SetData(&modelMatrix, sizeof(modelMatrix));
                m_GLRenderableObjects[j].VertexArray.Bind();
                m_GLRenderableObjects[j].IndexBuffer.Bind();

                OpenGL::GL::DrawElements(
                    OpenGL::DrawMode::Triangles,
                    static_cast<std::int32_t>(m_RenderableObjects[j].m_Mesh->GetIndexData().size()),
                    OpenGL::GL::GetGLType<std::uint32_t>(), nullptr);
            }
            index += m_RenderPassObjectCount[i];
        }

        std::chrono::high_resolution_clock::time_point newTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(newTime - currentTime).count();

#ifdef NEBULA_INCLUDE_IMGUI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Render Statistics");

        ImGui::Text("Renderer: OpenGL");
        ImGui::Text("Render Passes: %zu", m_RenderPasses.size());
        ImGui::Text("Renderable Objects: %zu", m_RenderableObjects.size());
        ImGui::Text("Frame Time: %f ms", static_cast<float>(elapsedTime) / 1000.0F);

        ImGui::End();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
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
