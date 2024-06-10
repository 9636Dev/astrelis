#include "OpenGLRenderer.hpp"

#include "NebulaEngine/Core/Utils/Function.hpp"
#include "NebulaEngine/Renderer/Vertex.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Nebula
{
    [[maybe_unused]] static void APIENTRY OpenGLMessageCallback(GLenum source,
                                                                GLenum type,
                                                                GLuint errorId,
                                                                GLenum severity,
                                                                GLsizei length,
                                                                const GLchar* message,
                                                                const void* userParam)
    {
        NEBULA_UNUSED(source);
        NEBULA_UNUSED(type);
        NEBULA_UNUSED(severity);
        NEBULA_UNUSED(length);
        NEBULA_UNUSED(userParam);
        NEBULA_CORE_LOG_ERROR("OpenGL Error: {0} ({1})", message, errorId);
    }

    OpenGLRenderer::OpenGLRenderer(Ref<Window> window, Bounds bounds) : m_Shader(glCreateProgram()), m_Bounds(bounds)
    {
        // To suppres the warning about using a const reference, in the future we need to store window for changing the context
        auto windowMoved = std::move(window);

        // TODO(9636D): Write this properly by abstracting OpenGL calls
        //glDebugMessageCallback(OpenGLMessageCallback, nullptr);
        //glEnable(GL_DEBUG_OUTPUT);
        //glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        NEBULA_UNUSED(windowMoved);

        glViewport(m_Bounds.X, m_Bounds.Y, m_Bounds.Width, m_Bounds.Height);

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void*>(offsetof(Vertex, Position)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              reinterpret_cast<void*>(offsetof(Vertex, Color)));

        glGenBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);


        // Shader
        const char* vertexShaderSource = R"(
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

        const char* fragmentShaderSource = R"(
            #version 330 core

            in vec4 v_Color;

            out vec4 o_Color;

            void main()
            {
                o_Color = v_Color;
            }
        )";

        GLint success       = 0;
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, infoLog.data());

            glDeleteShader(vertexShader);

            NEBULA_CORE_LOG_ERROR("Failed to compile vertex shader: {0}", infoLog.data());
            throw std::runtime_error("Failed to compile vertex shader");
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);

        if (success == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, infoLog.data());

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            NEBULA_CORE_LOG_ERROR("Failed to compile fragment shader: {0}", infoLog.data());
            throw std::runtime_error("Failed to compile fragment shader");
        }

        glAttachShader(m_Shader, vertexShader);
        glAttachShader(m_Shader, fragmentShader);
        glLinkProgram(m_Shader);

        glGetProgramiv(m_Shader, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_Shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_Shader, maxLength, &maxLength, infoLog.data());

            glDeleteProgram(m_Shader);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            NEBULA_CORE_LOG_ERROR("Failed to link shader program: {0}", infoLog.data());
            throw std::runtime_error("Failed to link shader program");
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    OpenGLRenderer::~OpenGLRenderer() = default;

    void OpenGLRenderer::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    void OpenGLRenderer::BeginFrame() { Clear(); }

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

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glUseProgram(m_Shader);

        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t), m_Indices.data(), GL_STREAM_DRAW);

        NEBULA_CORE_LOG_INFO("Drawing {0} vertices and {1} indices", m_Vertices.size(), m_Indices.size());
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr);

        m_Vertices.clear();
        m_Indices.clear();
    }

    Result<Ptr<OpenGLRenderer>, std::string> OpenGLRenderer::Create(Ref<Window> window, Bounds bounds)
    {
        if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
        {
            return "Failed to initialize Renderer API (GLAD - OpenGL)";
        }

        return MakePtr<OpenGLRenderer>(std::move(window), bounds);
    }
} // namespace Nebula
