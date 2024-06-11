#include "OpenGLRenderer.hpp"

#include "NebulaEngine/Core/Utils/Function.hpp"
#include "NebulaEngine/Platform/OpenGL/API/VertexBuffer.hpp"
#include "NebulaEngine/Renderer/Vertex.hpp"
#include <string>

#include "API/GL.hpp"

namespace Nebula
{
    OpenGLRenderer::OpenGLRenderer(Ref<Window> window, Bounds bounds) : m_Bounds(bounds)
    {
        // To suppres the warning about using a const reference, in the future we need to store window for changing the context
        auto windowMoved = std::move(window);

        NEBULA_UNUSED(windowMoved);

        {
            OpenGL::VertexBufferFormat format;
            format.Push<float>(3);
            format.Push<float>(4);
            m_VertexArray.AddVertexBuffer(m_VertexBuffer, format);
        }


        // Shader
        std::string vertexShaderSource = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec4 v_Color;

            void main()
            {
                gl_Position = vec4(a_Position, 1.0);
                v_Color = a_Color;
            }
        )";

        std::string fragmentShaderSource = R"(
            #version 330 core

            in vec4 v_Color;

            out vec4 o_Color;

            void main()
            {
                o_Color = v_Color;
            }
        )";

        OpenGL::Shader vertexShader(OpenGL::ShaderType::Vertex);
        vertexShader.SetSource(vertexShaderSource);
        if (!vertexShader.Compile())
        {
            NEBULA_CORE_LOG_ERROR("Vertex shader compilation failed: {0}", vertexShader.GetInfoLog());
            throw std::runtime_error("Vertex shader compilation failed");
        }

        OpenGL::Shader fragmentShader(OpenGL::ShaderType::Fragment);
        fragmentShader.SetSource(fragmentShaderSource);
        if (!fragmentShader.Compile())
        {
            NEBULA_CORE_LOG_ERROR("Fragment shader compilation failed: {0}", fragmentShader.GetInfoLog());
            throw std::runtime_error("Fragment shader compilation failed");
        }

        m_Program.AttachShader(vertexShader);
        m_Program.AttachShader(fragmentShader);

        if (!m_Program.Link())
        {
            NEBULA_CORE_LOG_ERROR("Program linking failed: {0}", m_Program.GetInfoLog());
            throw std::runtime_error("Program linking failed");
        }
    }

    OpenGLRenderer::~OpenGLRenderer() = default;

    void OpenGLRenderer::Viewport(const Bounds& bounds)
    {
        m_Bounds = bounds;
    }

    void OpenGLRenderer::Clear() { OpenGL::GL::Clear(OpenGL::ClearTarget::Color | OpenGL::ClearTarget::Depth); }

    void OpenGLRenderer::BeginFrame() {
        Clear();
        OpenGL::GL::Viewport(m_Bounds.X, m_Bounds.Y, m_Bounds.Width, m_Bounds.Height);
    }

    void OpenGLRenderer::EndFrame() { DrawBatch(); }

    void OpenGLRenderer::DrawMesh(const StaticMesh& mesh, const Transform& transform)
    {
        for (std::size_t i = 0; i < mesh.Vertices.size(); i++)
        {
            // We have no uniform buffer object, so we need to modify the vertices directly
            Vertex vertex = mesh.Vertices[i];
            vertex.Position[0] += transform.GetPosition().x();
            vertex.Position[1] += transform.GetPosition().y();
            vertex.Position[2] += transform.GetPosition().z();
            m_Vertices.push_back(vertex);
        };

        // We need to copy the indices
        for (std::size_t i = 0; i < mesh.Indices.size(); i++)
        {
            m_Indices.push_back(mesh.Indices[i] + m_Vertices.size() - mesh.Vertices.size());
        }

        if (m_Vertices.size() >= 1'000)
        {
            // Arbitrary number
            DrawBatch();
        }
    }

    void OpenGLRenderer::DrawBatch()
    {
        if (m_Vertices.empty())
        {
            return;
        }

        m_VertexArray.Bind();
        m_IndexBuffer.Bind();
        m_Program.Use();

        m_VertexBuffer.SetData(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex), OpenGL::BufferUsage::StreamDraw);
        m_IndexBuffer.SetData(m_Indices.data(), m_Indices.size(), OpenGL::BufferUsage::StreamDraw);

        NEBULA_CORE_LOG_INFO("Drawing {0} vertices and {1} indices", m_Vertices.size(), m_Indices.size());
        OpenGL::GL::DrawElements(OpenGL::DrawMode::Triangles, m_Indices.size(), OpenGL::GLType::UnsignedInt, nullptr);

        m_Vertices.clear();
        m_Indices.clear();
    }

    Result<Ptr<OpenGLRenderer>, std::string> OpenGLRenderer::Create(Ref<Window> window, Bounds bounds)
    {
        return MakePtr<OpenGLRenderer>(std::move(window), bounds);
    }
} // namespace Nebula
