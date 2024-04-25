#pragma once

#include "Enum.hpp"

#include <set>
#include <utility>

namespace Nebula::OpenGL
{
    template<typename T>
    concept GLType = std::same_as<T, std::uint32_t> || std::same_as<T, std::int32_t> || std::same_as<T, float> ||
                     std::same_as<T, double> || std::same_as<T, std::uint16_t> || std::same_as<T, std::int16_t> ||
                     std::same_as<T, std::uint8_t> || std::same_as<T, std::int8_t>;

    using VertexArray_t = std::uint32_t;
    using Buffer_t      = std::uint32_t;
    using Shader_t      = std::uint32_t;
    using Program_t     = std::uint32_t;
    using Texture_t     = std::uint32_t;
    using sizei_t       = std::int32_t;
    using sizeiptr_t    = std::int64_t;
    using intptr_t      = std::ptrdiff_t;

    struct GLVersion
    {
        std::int32_t Major;
        std::int32_t Minor;

        constexpr GLVersion(std::int32_t major, std::int32_t minor) noexcept : Major(major), Minor(minor) {}
    };

    struct GLFunctionCall
    {
        std::string Function;
        std::string File;
        std::uint32_t Line;

        GLFunctionCall(std::string function, std::string file, std::uint32_t line) :
            Function(std::move(function)),
            File(std::move(file)),
            Line(line)
        {
        }
    };

    class GL
    {
    public:
        static void Init(GLVersion& version);
        static void Terminate();

        template<GLType T> static constexpr Type GetGLType();

        static constexpr std::size_t GetSizeOfGLType(Type type)
        {
            switch (type)
            {
            case Type::Byte:
            case Type::UnsignedByte:
                return sizeof(std::uint8_t);
            case Type::Short:
            case Type::UnsignedShort:
                return sizeof(std::uint16_t);
            case Type::Int:
            case Type::UnsignedInt:
                return sizeof(std::uint32_t);
            case Type::Float:
                return sizeof(float);
            case Type::Double:
                return sizeof(double);
            case Type::HalfFloat:
                return sizeof(std::uint16_t);
            case Type::Fixed:
                return sizeof(std::uint32_t);
            }
        }

        static void ClearColor(float red, float green, float blue, float alpha);
        static void Clear(ClearTarget target);
        static void SetPolygonMode(PolygonFace face, PolygonMode mode);
        static void Enable(EnableTarget target);
        static void Disable(EnableTarget target);
        static void BlendFunc(BlendFactor sfactor, BlendFactor dfactor);
        static void Viewport(std::int32_t posX, std::int32_t posY, sizei_t width, sizei_t height);

        static void DrawElements(DrawMode mode, sizei_t count, Type type, const void* indices);
        static void
            DrawElementsInstanced(DrawMode mode, sizei_t count, Type type, const void* indices, sizei_t instancecount);
        static void DrawElementsInstancedBaseIntance(DrawMode mode,
                                                     sizei_t count,
                                                     Type type,
                                                     const void* indices,
                                                     sizei_t instanceCount,
                                                     std::uint32_t baseinstance);

        static void GenVertexArrays(sizei_t n, VertexArray_t* arrays);
        static void GenBuffers(sizei_t n, Buffer_t* buffers);
        static void GenTextures(sizei_t n, Texture_t* textures);

        static void DeleteVertexArrays(sizei_t n, const VertexArray_t* arrays);
        static void DeleteBuffers(sizei_t n, const Buffer_t* buffers);
        static void DeleteTextures(sizei_t n, const Texture_t* textures);

        static void BindVertexArray(VertexArray_t array);
        static void BindBuffer(BufferTarget target, Buffer_t buffer);
        static void BindBufferBase(BufferTarget target, std::uint32_t index, Buffer_t buffer);

        static void BufferData(BufferTarget target, sizeiptr_t size, const void* data, BufferUsage usage);
        static void BufferSubData(BufferTarget target, intptr_t offset, sizeiptr_t size, const void* data);

        static void EnableVertexAttribArray(std::uint32_t index);
        static void DisableVertexAttribArray(std::uint32_t index);
        static void VertexAttribPointer(
            std::uint32_t index, std::int32_t size, Type type, bool normalized, sizei_t stride, const void* pointer);
        static void VertexAttribLPointer(
            std::uint32_t index, std::int32_t size, Type type, sizei_t stride, const void* pointer);
        static void VertexAttribDivisor(std::uint32_t index, std::uint32_t divisor);

        [[nodiscard]] static Shader_t CreateShader(ShaderType type);
        static void DeleteShader(Shader_t shader);
        static void ShaderSource(Shader_t shader, sizei_t count, const char** string, const std::int32_t* length);
        static void CompileShader(Shader_t shader);
        static void GetShaderiv(Shader_t shader, ShaderParameter pname, std::int32_t* params);
        static void GetShaderInfoLog(Shader_t shader, sizei_t maxLength, sizei_t* length, char* infoLog);
        static void GetShaderSource(Shader_t shader, sizei_t maxLength, sizei_t* length, char* source);

        [[nodiscard]] static Program_t CreateProgram();
        static void DeleteProgram(Program_t program);
        static void AttachShader(Program_t program, Shader_t shader);
        static void DetachShader(Program_t program, Shader_t shader);
        static void LinkProgram(Program_t program);
        static void GetProgramiv(Program_t program, ProgramParameter pname, std::int32_t* params);
        static void GetProgramInfoLog(Program_t program, std::int32_t maxLength, std::int32_t* length, char* infoLog);
        static void UseProgram(Program_t program);

        static std::int32_t GetUniformLocation(Program_t program, const char* name);
        static void Uniform1i(std::int32_t location, std::int32_t value0);
        static void Uniform1f(std::int32_t location, float value0);
        static void Uniform2f(std::int32_t location, float value0, float value1);
        static void Uniform3f(std::int32_t location, float value0, float value1, float value2);
        static void Uniform4f(std::int32_t location, float value0, float value1, float value2, float value3);
        static void UniformMatrix3fv(std::int32_t location, std::int32_t count, bool transpose, const float* value);
        static void UniformMatrix4fv(std::int32_t location, std::int32_t count, bool transpose, const float* value);

        static void GetUniformfv(Program_t program, std::int32_t location, float* params);

        static std::uint32_t GetUniformBlockIndex(Program_t program, const char* uniformBlockName);
        static void UniformBlockBinding(Program_t program, std::uint32_t uniformBlockIndex, std::uint32_t uniformBlockBinding);

        static void BindTexture(TextureTarget target, std::uint32_t texture);
        static void TexImage2D(TextureTarget target,
                               std::int32_t level,
                               InternalFormat internalFormat,
                               std::int32_t width,
                               std::int32_t height,
                               std::int32_t border,
                               PixelFormat format,
                               PixelType type,
                               const void* data);
        static void ActiveTexture(TextureUnit unit);
        static void TexParameteri(TextureTarget target, TextureParameter pname, std::int32_t param);

        [[nodiscard]] static Program_t GetBoundProgram();
        [[nodiscard]] static VertexArray_t GetBoundVertexArray();
        static void SetLogLevel(std::uint32_t logLevel);

        inline static const GLVersion& GetVersion() { return s_Version; }
    private:
        static void NebulaGLCallback(std::uint32_t source,
                                     std::uint32_t type,
                                     std::uint32_t msgId,
                                     std::uint32_t severity,
                                     sizei_t msg_length,
                                     const char* message,
                                     const void* userParam);
        static void CheckError(const std::string& function, const std::string& file, std::uint32_t line);

        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        static std::set<VertexArray_t> s_VertexArrays;
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        static std::set<Buffer_t> s_Buffers;
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        static GLVersion s_Version;
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        static GLFunctionCall s_LastFunction;
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        static std::uint32_t s_LogLevel;
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        static bool s_IsUsingDebugCallback;
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        static VertexArray_t s_BoundVertexArray;
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        static Program_t s_BoundProgram;
    };

    template<> inline Type GL::GetGLType<float>() { return Type::Float; }

    template<> inline Type GL::GetGLType<double>() { return Type::Double; }

    template<> inline Type GL::GetGLType<std::int32_t>() { return Type::Int; }

    template<> inline Type GL::GetGLType<std::uint32_t>() { return Type::UnsignedInt; }

    template<> inline Type GL::GetGLType<std::int16_t>() { return Type::Short; }

    template<> inline Type GL::GetGLType<std::uint16_t>() { return Type::UnsignedShort; }

    template<> inline Type GL::GetGLType<std::int8_t>() { return Type::Byte; }

    template<> inline Type GL::GetGLType<std::uint8_t>() { return Type::UnsignedByte; }

} // namespace Nebula::OpenGL
