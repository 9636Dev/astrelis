#include "GLShader.hpp"

#include "GLAPI.hpp"

namespace Nebula::OpenGL
{
    GLShader::GLShader(ShaderType type)
        : m_Id(GL::CreateShader(type)) {}

    GLShader::~GLShader()
    {
        GL::DeleteShader(m_Id);
    }

    GLShader::GLShader(GLShader&& other) noexcept
        : m_Id(other.m_Id)
    {
        other.m_Id = 0;
    }

    GLShader& GLShader::operator=(GLShader&& other) noexcept
    {
        if (this != &other)
        {
            GL::DeleteShader(m_Id);
            m_Id = other.m_Id;
            other.m_Id = 0;
        }
        return *this;
    }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    void GLShader::SetSource(std::string_view source)
    {
        const char* src = source.data();
        GL::ShaderSource(m_Id, 1, &src, nullptr);
    }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    bool GLShader::Compile()
    {
        GL::CompileShader(m_Id);

        int status = 0;
        GL::GetShaderiv(m_Id, ShaderParameter::CompileStatus, &status);

        return status != 0;
    }

    std::string GLShader::GetInfoLog() const
    {
        int length = 0;
        GL::GetShaderiv(m_Id, ShaderParameter::InfoLogLength, &length);

        std::string infoLog(length, '\0');
        GL::GetShaderInfoLog(m_Id, length, nullptr, infoLog.data());

        return infoLog;
    }

    GLShaderProgram::GLShaderProgram()
        : m_Id(GL::CreateProgram()) {}

    GLShaderProgram::~GLShaderProgram()
    {
        GL::DeleteProgram(m_Id);
    }

    GLShaderProgram::GLShaderProgram(GLShaderProgram&& other) noexcept
        : m_Id(other.m_Id)
    {
        other.m_Id = 0;
    }

    GLShaderProgram& GLShaderProgram::operator=(GLShaderProgram&& other) noexcept
    {
        if (this != &other)
        {
            GL::DeleteProgram(m_Id);
            m_Id = other.m_Id;
            other.m_Id = 0;
        }
        return *this;
    }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    void GLShaderProgram::AttachShader(const GLShader& shader)
    {
        GL::AttachShader(m_Id, shader.m_Id);
    }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    bool GLShaderProgram::Link()
    {
        GL::LinkProgram(m_Id);

        int status = 0;
        GL::GetProgramiv(m_Id, ProgramParameter::LinkStatus, &status);
        return status != 0;
    }

    std::string GLShaderProgram::GetInfoLog() const
    {
        int length = 0;
        GL::GetProgramiv(m_Id, ProgramParameter::InfoLogLength, &length);

        std::string infoLog(length, '\0');
        GL::GetProgramInfoLog(m_Id, length, nullptr, infoLog.data());

        return infoLog;
    }

    void GLShaderProgram::Bind() const
    {
        GL::UseProgram(m_Id);
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void GLShaderProgram::Unbind() const
    {
        GL::UseProgram(0);
    }

    void GLShaderProgram::SetMat4(std::string_view name, Matrix4f value)
    {
        GL::UniformMatrix4fv(GetUniformLocation(name), 1, false, value.data());
    }

    std::int32_t GLShaderProgram::GetUniformLocation(std::string_view name)
    {
        auto iter = m_UniformLocations.find(name);
        if (iter != m_UniformLocations.end())
        {
            return iter->second;
        }

        int location = GL::GetUniformLocation(m_Id, name.data());
        m_UniformLocations[name] = location;
        return location;
    }

    Ptr<GLShaderProgram> GLShaderProgram::Create(std::string name, std::string_view vertexSource, std::string_view fragmentSource)
    {
        auto program = MakePtr<GLShaderProgram>();
        GLShader vertexShader(ShaderType::Vertex);
        vertexShader.SetSource(vertexSource);
        if (!vertexShader.Compile())
        {
            NEBULA_LOG_ERROR("Vertex shader compilation failed: {0}", vertexShader.GetInfoLog());
            return nullptr;
        }
        program->AttachShader(vertexShader);

        GLShader fragmentShader(ShaderType::Fragment);
        fragmentShader.SetSource(fragmentSource);
        if (!fragmentShader.Compile())
        {
            NEBULA_LOG_ERROR("Fragment shader compilation failed: {0}", fragmentShader.GetInfoLog());
            return nullptr;
        }
        program->AttachShader(fragmentShader);

        if (!program->Link())
        {
            NEBULA_LOG_ERROR("Shader program linking failed: {0}", program->GetInfoLog());
            return nullptr;
        }

        program->m_Name = std::move(name);
        return program;
    }
} // namespace Nebula::OpenGL
