#pragma once

#include "NebulaEngine/Core/Math.hpp"
#include "NebulaEngine/Platform/OpenGL/API/Enum.hpp"
#include <cstdint>
#include <string>
#include <string_view>

namespace Nebula::OpenGL
{
    class Shader
    {
    public:
        explicit Shader(ShaderType type);
        ~Shader();
        Shader(const Shader&)            = delete;
        Shader& operator=(const Shader&) = delete;
        Shader(Shader&& other) noexcept;
        Shader& operator=(Shader&& other) noexcept;

        void SetSource(std::string_view source);
        bool Compile();
        std::string GetInfoLog() const;
    private:
        std::uint32_t m_Id;

        friend class Program;
    };

    class Program
    {
    public:
        Program();
        ~Program();
        Program(const Program&)            = delete;
        Program& operator=(const Program&) = delete;
        Program(Program&& other) noexcept;
        Program& operator=(Program&& other) noexcept;

        void AttachShader(const Shader& shader);
        bool Link();
        std::string GetInfoLog() const;

        void Use() const;
        void Unuse() const;

        void SetUniform(std::string_view name, Vector4f value);

    private:
        std::int32_t GetUniformLocation(std::string_view name);

        std::uint32_t m_Id;
        std::unordered_map<std::string_view, int> m_UniformLocations;
    };
} // namespace Nebula::OpenGL
