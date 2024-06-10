#pragma once

#include <cstdint>
#include <string_view>

#include "Enum.hpp"

namespace Nebula::OpenGL
{
    struct GLVersion
    {
        int Major;
        int Minor;

        GLVersion(int major, int minor)
            : Major(major), Minor(minor) {}
    };

    struct InitInfo
    {
        GLVersion Version;
        bool DebugCallback;

        InitInfo(GLVersion version, bool debugCallback)
            : Version(version), DebugCallback(debugCallback) {}
    };

    class GL
    {
    public:
        static InitInfo Init();

        static GLVersion GetVersion();

        static void Clear(ClearTarget target);
        static void Viewport(int posX, int posY, int width, int height);

        static void DrawElements(DrawMode mode, std::size_t count, GLType type, const void* indices);

        static void GenVertexArrays(std::uint32_t n, std::uint32_t* arrays);
        static void BindVertexArray(std::uint32_t array);
        static void DeleteVertexArrays(std::uint32_t n, const std::uint32_t* arrays);
        static void EnableVertexAttribArray(std::uint32_t index);
        static void VertexAttribPointer(std::uint32_t index, std::uint32_t size, GLType type, bool normalized, std::uint32_t stride, const void* pointer);

        static void GenBuffers(std::uint32_t n, std::uint32_t* buffers);
        static void BindBuffer(BufferTarget target, std::uint32_t buffer);
        static void BufferData(BufferTarget target, std::size_t size, const void* data, BufferUsage usage);
        static void DeleteBuffers(std::uint32_t n, const std::uint32_t* buffers);

        static std::uint32_t CreateShader(ShaderType type);
        static void ShaderSource(std::uint32_t shader, std::uint32_t count, const char** string, const int* length);
        static void CompileShader(std::uint32_t shader);
        static void GetShaderiv(std::uint32_t shader, ShaderParameter pname, int* params);
        static void GetShaderInfoLog(std::uint32_t shader, int maxLength, int* length, char* infoLog);
        static void DeleteShader(std::uint32_t shader);

        static std::uint32_t CreateProgram();
        static void AttachShader(std::uint32_t program, std::uint32_t shader);
        static void LinkProgram(std::uint32_t program);
        static void GetProgramiv(std::uint32_t program, ProgramParameter pname, int* params);
        static void GetProgramInfoLog(std::uint32_t program, int maxLength, int* length, char* infoLog);
        static void UseProgram(std::uint32_t program);
        static void DeleteProgram(std::uint32_t program);
    private:
        struct CallLocation
        {
            std::string_view File;
            int Line;
            std::string_view Function;
        };
        static void GLCallHelper(std::string_view file, int line, std::string_view function);
        static void EnableDebugCallback();
        static void GLDebugMessageCallback(std::uint32_t source, std::uint32_t type, std::uint32_t errId, std::uint32_t severity, int length, const char* message, const void* userParam);

        static bool s_DebugCallback;
        static CallLocation s_CallLocation;
    };
} // namespace Nebula::OpenGL
