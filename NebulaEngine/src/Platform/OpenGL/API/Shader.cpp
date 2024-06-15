#include "Shader.hpp"

#include "GL.hpp"

namespace Nebula::OpenGL
{
    Shader::Shader(ShaderType type)
        : m_Id(GL::CreateShader(type)) {}

    Shader::~Shader()
    {
        GL::DeleteShader(m_Id);
    }

    Shader::Shader(Shader&& other) noexcept
        : m_Id(other.m_Id)
    {
        other.m_Id = 0;
    }

    Shader& Shader::operator=(Shader&& other) noexcept
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
    void Shader::SetSource(std::string_view source)
    {
        const char* src = source.data();
        GL::ShaderSource(m_Id, 1, &src, nullptr);
    }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    bool Shader::Compile()
    {
        GL::CompileShader(m_Id);

        int status = 0;
        GL::GetShaderiv(m_Id, ShaderParameter::CompileStatus, &status);

        return status != 0;
    }

    std::string Shader::GetInfoLog() const
    {
        int length = 0;
        GL::GetShaderiv(m_Id, ShaderParameter::InfoLogLength, &length);

        std::string infoLog(length, '\0');
        GL::GetShaderInfoLog(m_Id, length, nullptr, infoLog.data());

        return infoLog;
    }

    Program::Program()
        : m_Id(GL::CreateProgram()) {}

    Program::~Program()
    {
        GL::DeleteProgram(m_Id);
    }

    Program::Program(Program&& other) noexcept
        : m_Id(other.m_Id)
    {
        other.m_Id = 0;
    }

    Program& Program::operator=(Program&& other) noexcept
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
    void Program::AttachShader(const Shader& shader)
    {
        GL::AttachShader(m_Id, shader.m_Id);
    }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    bool Program::Link()
    {
        GL::LinkProgram(m_Id);

        int status = 0;
        GL::GetProgramiv(m_Id, ProgramParameter::LinkStatus, &status);
        return status != 0;
    }

    std::string Program::GetInfoLog() const
    {
        int length = 0;
        GL::GetProgramiv(m_Id, ProgramParameter::InfoLogLength, &length);

        std::string infoLog(length, '\0');
        GL::GetProgramInfoLog(m_Id, length, nullptr, infoLog.data());

        return infoLog;
    }

    void Program::Use() const
    {
        GL::UseProgram(m_Id);
    }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void Program::Unuse() const
    {
        GL::UseProgram(0);
    }

    std::int32_t Program::GetUniformLocation(std::string_view name)
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

    // NOLINTNEXTLINE(readability-make-member-function-const)
    void Program::SetUniform(std::string_view name, Vector4f value)
    {
        GL::Uniform4f(GetUniformLocation(name), value.x(), value.y(), value.z(), value.w());
    }
} // namespace Nebula::OpenGL
