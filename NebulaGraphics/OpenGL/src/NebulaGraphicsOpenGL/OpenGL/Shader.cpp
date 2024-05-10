#include "Shader.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace Nebula::OpenGL
{
    Shader::Shader(ShaderType type) : m_Shader(GL::CreateShader(type)), m_Type(type) {}

    Shader::Shader(Shader&& other) noexcept : m_Shader(other.m_Shader), m_Type(other.m_Type) { other.m_Shader = 0; }

    Shader& Shader::operator=(Shader&& other) noexcept
    {
        m_Shader       = other.m_Shader;
        m_Type         = other.m_Type;
        other.m_Shader = 0;
        return *this;
    }

    Shader::~Shader() { GL::DeleteShader(m_Shader); }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    void Shader::ShaderSource(const std::string& source)
    {
        const char* src = source.c_str();
        GL::ShaderSource(m_Shader, 1, &src, nullptr);
    }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    bool Shader::Compile()
    {
        GL::CompileShader(m_Shader);
        std::int32_t status = 0;
        GL::GetShaderiv(m_Shader, ShaderParameter::CompileStatus, &status);
        return status != 0;
    }

    std::string Shader::GetInfoLog() const
    {
        std::int32_t length = 0;
        GL::GetShaderiv(m_Shader, ShaderParameter::InfoLogLength, &length);
        std::string infoLog(length, '\0');
        GL::GetShaderInfoLog(m_Shader, length, &length, infoLog.data());
        return infoLog;
    }

    std::string Shader::GetSource() const
    {
        std::int32_t length = 0;
        GL::GetShaderiv(m_Shader, ShaderParameter::ShaderSourceLength, &length);
        std::string source(length, '\0');
        GL::GetShaderSource(m_Shader, length, &length, source.data());
        return source;
    }

    ShaderType Shader::GetType() const { return m_Type; }

    ShaderProgram::ShaderProgram() : m_Program(GL::CreateProgram()) {}

    ShaderProgram::~ShaderProgram() { GL::DeleteProgram(m_Program); }

    ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept : m_Program(other.m_Program) { other.m_Program = 0; }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
    {
        m_Program       = other.m_Program;
        other.m_Program = 0;
        return *this;
    }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    void ShaderProgram::AttachShader(const Shader& shader) { GL::AttachShader(m_Program, shader.m_Shader); }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    void ShaderProgram::DetachShader(const Shader& shader) { GL::DetachShader(m_Program, shader.m_Shader); }

    // NOLINTNEXTLINE(readability-make-member-function-const)
    bool ShaderProgram::Link()
    {
        GL::LinkProgram(m_Program);
        std::int32_t status = 0;
        GL::GetProgramiv(m_Program, ProgramParameter::LinkStatus, &status);
        return status != 0;
    }

    void ShaderProgram::Use() const { GL::UseProgram(m_Program); }

    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void ShaderProgram::Unuse() const { GL::UseProgram(0); }

    std::string ShaderProgram::GetInfoLog() const
    {
        std::int32_t length = 0;
        GL::GetProgramiv(m_Program, ProgramParameter::InfoLogLength, &length);
        std::string infoLog(length, '\0');
        GL::GetProgramInfoLog(m_Program, length, &length, infoLog.data());
        return infoLog;
    }

    std::int32_t ShaderProgram::GetUniformLocation(const std::string& name) const
    {
        if (m_UniformLocations.find(name) != m_UniformLocations.end())
        {
            return m_UniformLocations[name];
        }
        std::int32_t location    = GL::GetUniformLocation(m_Program, name.c_str());
        m_UniformLocations[name] = location;
        return location;
    }

    std::uint32_t ShaderProgram::GetUniformBlockIndex(const std::string& name) const
    {
        return GL::GetUniformBlockIndex(m_Program, name.c_str());
    }

    void ShaderProgram::UniformBlockBinding(std::uint32_t uniformBlockIndex, std::uint32_t uniformBlockBinding)
    {
        GL::UniformBlockBinding(m_Program, uniformBlockIndex, uniformBlockBinding);
    }

    void ShaderProgram::SetUniform(const std::string& name, std::int32_t value)
    {
        // Don't bind the program here, because it might be bound already for performance reasons
        NEB_ASSERT(GL::GetBoundProgram() == m_Program, "ShaderProgram::SetUniform: ShaderProgram is not bound!");
        GL::Uniform1i(GetUniformLocation(name), value);
    }

    void ShaderProgram::SetUniform(const std::string& name, float value)
    {
        NEB_ASSERT(GL::GetBoundProgram() == m_Program, "ShaderProgram::SetUniform: ShaderProgram is not bound!");
        GL::Uniform1f(GetUniformLocation(name), value);
    }

    void ShaderProgram::SetUniform(const std::string& name, const glm::vec2& value)
    {
        NEB_ASSERT(GL::GetBoundProgram() == m_Program, "ShaderProgram::SetUniform: ShaderProgram is not bound!");
        GL::Uniform2f(GetUniformLocation(name), value.x, value.y);
    }

    void ShaderProgram::SetUniform(const std::string& name, const glm::vec3& value)
    {
        NEB_ASSERT(GL::GetBoundProgram() == m_Program, "ShaderProgram::SetUniform: ShaderProgram is not bound!");
        GL::Uniform3f(GetUniformLocation(name), value.x, value.y, value.z);
    }

    void ShaderProgram::SetUniform(const std::string& name, const glm::vec4& value)
    {
        NEB_ASSERT(GL::GetBoundProgram() == m_Program, "ShaderProgram::SetUniform: ShaderProgram is not bound!");
        GL::Uniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void ShaderProgram::SetUniform(const std::string& name, const glm::mat3& value)
    {
        NEB_ASSERT(GL::GetBoundProgram() == m_Program, "ShaderProgram::SetUniform: ShaderProgram is not bound!");
        GL::UniformMatrix3fv(GetUniformLocation(name), 1, false, glm::value_ptr(value));
    }

    void ShaderProgram::SetUniform(const std::string& name, const glm::mat4& value)
    {
        NEB_ASSERT(GL::GetBoundProgram() == m_Program, "ShaderProgram::SetUniform: ShaderProgram is not bound!");
        GL::UniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(value));
    }
} // namespace Nebula::OpenGL
