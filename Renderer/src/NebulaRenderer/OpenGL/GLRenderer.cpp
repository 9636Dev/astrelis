#include "GLRenderer.hpp"

#include "NebulaRenderer/Renderer.hpp"
#include "OpenGL/GL.hpp"
#include "imgui.h"
#include <GL/glew.h>

namespace Nebula
{
    GLRenderer::GLRenderer(const RenderingContext& context) noexcept : m_Window(context.Window), m_Dimensions(context.Dimensions)
    {
        OpenGL::GL::Init();

        // This is all temporary, just to test if the renderer works
        float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);
        glGenBuffers(1, &m_VertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        const char* vertexShaderSource = R"(
            #version 330 core
            layout(location = 0) in vec3 aPos;
            void main()
            {
                gl_Position = vec4(aPos, 1.0);
            }
        )";

        const char* fragmentShaderSource = R"(
            #version 330 core
            out vec4 FragColor;
            void main()
            {
                FragColor = vec4(1.0, 0.5, 0.2, 1.0);
            }
        )";

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        m_ShaderProgram = glCreateProgram();
        glAttachShader(m_ShaderProgram, vertexShader);
        glAttachShader(m_ShaderProgram, fragmentShader);
        glLinkProgram(m_ShaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glGenFramebuffers(1, &m_FrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

        glGenTextures(1, &m_Texture);
        glBindTexture(GL_TEXTURE_2D, m_Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1'280 / 2, 720 / 2, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

        glGenRenderbuffers(1, &m_RenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1'280 / 2, 720 / 2);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            NEBULA_CORE_LOG_ERROR("Framebuffer is not complete!");
        }
    }

    void GLRenderer::Resize(Dimension dimensions)
    {
        m_Dimensions = dimensions;
        glDeleteFramebuffers(1, &m_FrameBuffer);
        glDeleteTextures(1, &m_Texture);
        glDeleteRenderbuffers(1, &m_RenderBuffer);

        glGenTextures(1, &m_Texture);
        glBindTexture(GL_TEXTURE_2D, m_Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<int>(m_Dimensions.Width), static_cast<int>(m_Dimensions.Height), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenRenderbuffers(1, &m_RenderBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<int>(m_Dimensions.Width), static_cast<int>(m_Dimensions.Height));
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glGenFramebuffers(1, &m_FrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            NEBULA_CORE_LOG_ERROR("Framebuffer is not complete!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    GLRenderer::~GLRenderer() { OpenGL::GL::Shutdown(); }

    void GLRenderer::SetClearColor(float red, float green, float blue, float alpha)
    {
        OpenGL::GL::ClearColor(red, green, blue, alpha);
    }

    void GLRenderer::NewFrame()
    {
        OpenGL::GL::Clear(OpenGL::ClearBufferMask::ColorBufferBit | OpenGL::ClearBufferMask::DepthBufferBit);
    }

    void GLRenderer::Render() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
        glViewport(0, 0, static_cast<int>(m_Dimensions.Width), static_cast<int>(m_Dimensions.Height));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(m_ShaderProgram);
        glBindVertexArray(m_VertexArray);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Create ImGui window and render into it
        ImGui::Begin("Renderer Preview");
        static ImVec2 size = ImGui::GetWindowSize();
        if (ImGui::GetWindowSize().x != size.x || ImGui::GetWindowSize().y != size.y) {
            size = ImGui::GetWindowSize();
            Resize({ static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y) });
        }
        ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(m_Texture)), ImVec2(static_cast<float>(m_Dimensions.Width), static_cast<float>(m_Dimensions.Height)));
        ImGui::End();
    }
    Result<Ptr<Renderer>, RendererCreationError> CreateRenderer(const RenderingContext& context)
    {
        // Check if the Window is compatible with the Renderer
        return MakePtr<GLRenderer>(context).Cast<Renderer>();
    }
} // namespace Nebula
