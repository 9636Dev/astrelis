#include "OpenGLRenderer.hpp"

#include "NebulaEngine/Core/Utils/Function.hpp"
#include "NebulaEngine/Renderer/Vertex.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Nebula
{
    OpenGLRenderer::OpenGLRenderer(Ref<Window> window, Bounds bounds) : m_Shader(glCreateProgram()), m_Bounds(bounds)
    {
        // To suppres the warning about using a const reference, in the future we need to store window for changing the context
        auto windowMoved = std::move(window);
        NEBULA_UNUSED(windowMoved);

        glViewport(m_Bounds.X, m_Bounds.Y, m_Bounds.Width, m_Bounds.Height);

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color)));

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

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        GLint success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);
            NEBULA_CORE_LOG_ERROR("Vertex Shader Compilation Error: {0}", infoLog);
        }

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        glLinkProgram(m_Shader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
            NEBULA_CORE_LOG_ERROR("Fragment Shader Compilation Error: {0}", infoLog);
        }

        GLenum error = glGetError();
        while (error != GL_NO_ERROR)
        {
            NEBULA_CORE_LOG_ERROR("OpenGL Error: {0}", error);
            error = glGetError();
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    OpenGLRenderer::~OpenGLRenderer() = default;

    void OpenGLRenderer::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    void OpenGLRenderer::BeginFrame() { Clear(); }

    void OpenGLRenderer::EndFrame()
    {
        DrawBatch();
    }

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

        m_Indices.insert(m_Indices.end(), mesh.Indices.begin(), mesh.Indices.end());

        DrawBatch();
    }

    void OpenGLRenderer::DrawBatch()
    {
        if (m_Vertices.empty())
        {
            return;
        }

        glBindVertexArray(m_VAO);
        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_DYNAMIC_DRAW);

        GLuint error = glGetError();
        while (error != GL_NO_ERROR)
        {
            NEBULA_CORE_LOG_ERROR("OpenGL Error: {0}", error);
            error = glGetError();
        }


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(std::uint32_t), m_Indices.data(), GL_DYNAMIC_DRAW);

        error = glGetError();
        while (error != GL_NO_ERROR)
        {
            NEBULA_CORE_LOG_ERROR("OpenGL Error: {0}", error);
            error = glGetError();
        }

        glUseProgram(m_Shader);

        GLint shader;
        glGetIntegerv(GL_CURRENT_PROGRAM, &shader);
        if (shader != (int)m_Shader)
        {
            NEBULA_CORE_LOG_ERROR("Shader not loaded correctly");
        }

        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &shader);
        if (shader != (int)m_VAO)
        {
            NEBULA_CORE_LOG_ERROR("VAO not loaded correctly");
        }

        glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_BINDING, &shader);
        if (shader != (int)m_VBO)
        {
            NEBULA_CORE_LOG_ERROR("VBO not loaded correctly");
        }

        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_BINDING, &shader);
        if (shader != (int)m_IBO)
        {
            NEBULA_CORE_LOG_ERROR("IBO not loaded correctly");
        }


        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr);

        error = glGetError();
        while (error != GL_NO_ERROR)
        {
            NEBULA_CORE_LOG_ERROR("OpenGL Error: {0}", error);
            error = glGetError();
        }


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
