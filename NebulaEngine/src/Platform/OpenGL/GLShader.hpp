#pragma once

#include "GLEnum.hpp"
#include "NebulaEngine/Core/Math.hpp"
#include "NebulaEngine/Renderer/Shader.hpp"

#include <cstdint>
#include <string>
#include <string_view>

namespace Nebula::OpenGL
{
    class GLShader
    {
    public:
        friend class GLShaderProgram;
        explicit GLShader(ShaderType type);
        ~GLShader();
        GLShader(const GLShader&)            = delete;
        GLShader& operator=(const GLShader&) = delete;
        GLShader(GLShader&& other) noexcept;
        GLShader& operator=(GLShader&& other) noexcept;

        void SetSource(std::string_view source);
        bool Compile();
        std::string GetInfoLog() const;
    private:
        std::uint32_t m_Id;
    };

    class GLShaderProgram : public ::Nebula::Shader
    {
    public:
        GLShaderProgram();
        ~GLShaderProgram() override;
        GLShaderProgram(const GLShaderProgram&)            = delete;
        GLShaderProgram& operator=(const GLShaderProgram&) = delete;
        GLShaderProgram(GLShaderProgram&& other) noexcept;
        GLShaderProgram& operator=(GLShaderProgram&& other) noexcept;

        void Bind() const override;
        void Unbind() const override;

        static Ptr<GLShaderProgram> Create(std::string name, std::string_view vertexSource, std::string_view fragmentSource);

        void AttachShader(const GLShader& shader);
        bool Link();
        std::string GetInfoLog() const;

        void SetMat4(std::string_view name, Matrix4f value) override;
    private:
        std::int32_t GetUniformLocation(std::string_view name);
        std::uint32_t m_Id;
        std::string m_Name;
        std::unordered_map<std::string_view, int> m_UniformLocations;
    };
} // namespace Nebula::OpenGL
