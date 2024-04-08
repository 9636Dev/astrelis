#pragma once

#include "GL.hpp"
#include <GL/glew.h>

namespace Nebula::OpenGL
{
#ifdef NEBULA_DEBUG
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define NEB_GLCall(func, ...)                  \
        switch (0)                                 \
        {                                          \
        default: {                                 \
            func(__VA_ARGS__);                     \
            CheckError(#func, __FILE__, __LINE__); \
        }                                          \
        }
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define NEB_GLCallR(func, ...)                 \
        [&]() {                                    \
            auto result = func(__VA_ARGS__);       \
            CheckError(#func, __FILE__, __LINE__); \
            return result;                         \
        }()
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define NEB_GLCall(func, ...)  func(__VA_ARGS__)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define NEB_GLCallR(func, ...) func(__VA_ARGS__)
#endif

    // STL types shouldn't crash at the start, unless out of memory
    //
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables, cert-err58-cpp)
    std::set<VertexArray_t> GL::s_VertexArrays;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables, cert-err58-cpp)
    std::set<Buffer_t> GL::s_Buffers;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    GLVersion GL::s_Version = {0, 0};
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables, cert-err58-cpp)
    GLFunctionCall GL::s_LastFunction("null", __FILE__, __LINE__);
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    bool GL::s_IsUsingDebugCallback = false;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    std::uint32_t GL::s_LogLevel = 1;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    VertexArray_t GL::s_BoundVertexArray = 0;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    Program_t GL::s_BoundProgram = 0;

    void GL::Init(GLVersion& version)
    {
        static bool s_Initialized = false;
        if (s_Initialized)
        {
            version = s_Version;
            return;
        }

        GLenum result = glewInit();
        if (result != GLEW_OK)
        {
            NEB_CORE_LOG_ERROR("[GL] Could not initialize GLEW! (Enum value: {})", result);

            version.Major = 0;
            version.Minor = 0;
            return; // Don't abort here, because the handling will be down by the window class.
        }

        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        const char* versionString = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        NEB_CORE_LOG_INFO("[GL] Using OpenGL version: {}", versionString);

        glGetIntegerv(GL_MAJOR_VERSION, &version.Major);
        glGetIntegerv(GL_MINOR_VERSION, &version.Minor);

        NEB_CORE_LOG_DEBUG("[GL] Initialized OpenGL version {}.{}", version.Major, version.Minor);
        s_Version     = version;
        s_Initialized = true;

        // Callbacks
        if (s_Version.Major >= 4 && s_Version.Minor >= 3)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(NebulaGLCallback, nullptr);
            s_IsUsingDebugCallback = true;
            NEB_CORE_LOG_DEBUG("[GL] Using OpenGL 4.3+ debug callback");
        }
        else if (glewIsSupported("GL_ARB_debug_output") != GL_FALSE)
        {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
            glDebugMessageCallbackARB(NebulaGLCallback, nullptr);
            s_IsUsingDebugCallback = true;
            NEB_CORE_LOG_DEBUG("[GL] Using OpenGL 4.3 ARB debug callback");
        }
        else
        {
            NEB_CORE_LOG_INFO("[GL] Could not enable debug callback!");
            s_IsUsingDebugCallback = false;
        }
    }

    void GL::Terminate() { NEB_CORE_LOG_INFO("[GL] Terminating OpenGL"); }

    void GL::ClearColor(float red, float green, float blue, float alpha)
    {
        NEB_GLCall(glClearColor, red, green, blue, alpha);
    }

    void GL::Clear(ClearTarget target) { NEB_GLCall(glClear, static_cast<std::uint32_t>(target)); }

    void GL::SetPolygonMode(PolygonFace face, PolygonMode mode)
    {
        NEB_GLCall(glPolygonMode, static_cast<std::uint32_t>(face), static_cast<std::uint32_t>(mode));
    }

    void GL::Enable(EnableTarget target) { NEB_GLCall(glEnable, static_cast<std::uint32_t>(target)); }

    void GL::Disable(EnableTarget target) { NEB_GLCall(glDisable, static_cast<std::uint32_t>(target)); }

    void GL::BlendFunc(BlendFactor sfactor, BlendFactor dfactor)
    {
        NEB_GLCall(glBlendFunc, static_cast<std::uint32_t>(sfactor), static_cast<std::uint32_t>(dfactor));
    }

    void GL::Viewport(std::int32_t posX, std::int32_t posY, sizei_t width, sizei_t height)
    {
        NEB_GLCall(glViewport, posX, posY, width, height);
    }

    void GL::DrawElements(DrawMode mode, sizei_t count, Type type, const void* indices)
    {
        NEB_GLCall(glDrawElements, static_cast<std::uint32_t>(mode), count, static_cast<std::uint32_t>(type), indices);
    }

    void GL::DrawElementsInstanced(DrawMode mode, sizei_t count, Type type, const void* indices, sizei_t instanceCount)
    {
        NEB_GLCall(glDrawElementsInstanced, static_cast<std::uint32_t>(mode), count, static_cast<std::uint32_t>(type),
                   indices, instanceCount);
    }

    void GL::DrawElementsInstancedBaseIntance(
        DrawMode mode, sizei_t count, Type type, const void* indices, sizei_t instanceCount, std::uint32_t baseInstance)
    {
        NEB_GLCall(glDrawElementsInstancedBaseInstance, static_cast<std::uint32_t>(mode), count,
                   static_cast<std::uint32_t>(type), indices, instanceCount, baseInstance);
    }

    void GL::GenVertexArrays(sizei_t n, VertexArray_t* arrays)
    {
        NEB_GLCall(glGenVertexArrays, n, arrays);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        NEB_RUN_IF_DEBUG(for (sizei_t i = 0; i < n; i++) { s_VertexArrays.insert(arrays[i]));
}
    }

    void GL::DeleteVertexArrays(sizei_t n, const VertexArray_t* arrays)
    {
        NEB_GLCall(glDeleteVertexArrays, n, arrays);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        NEB_RUN_IF_DEBUG(for (sizei_t i = 0; i < n; i++) { s_VertexArrays.erase(arrays[i]));
}
    }

    void GL::BindVertexArray(VertexArray_t array)
    {
        if (s_BoundVertexArray == array)
        {
            return;
        }
        s_BoundVertexArray = array;
        NEB_GLCall(glBindVertexArray, array);
    }

    void GL::GenBuffers(sizei_t n, Buffer_t* buffers)
    {
        NEB_GLCall(glGenBuffers, n, buffers);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        NEB_RUN_IF_DEBUG(for (sizei_t i = 0; i < n; i++) { s_Buffers.insert(buffers[i]));
}
    }

    void GL::DeleteBuffers(sizei_t n, const Buffer_t* buffers)
    {
        NEB_GLCall(glDeleteBuffers, n, buffers);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        NEB_RUN_IF_DEBUG(for (sizei_t i = 0; i < n; i++) { s_Buffers.erase(buffers[i]));
}
    }

    void GL::BindBuffer(BufferTarget target, Buffer_t buffer)
    {
        NEB_GLCall(glBindBuffer, static_cast<std::uint32_t>(target), buffer);
    }

    void GL::BufferData(BufferTarget target, sizeiptr_t size, const void* data, BufferUsage usage)
    {
        NEB_GLCall(glBufferData, static_cast<std::uint32_t>(target), size, data, static_cast<std::uint32_t>(usage));
    }

    void GL::BufferSubData(BufferTarget target, intptr_t offset, sizeiptr_t size, const void* data)
    {
        NEB_GLCall(glBufferSubData, static_cast<std::uint32_t>(target), offset, size, data);
    }

    void GL::EnableVertexAttribArray(std::uint32_t index) { NEB_GLCall(glEnableVertexAttribArray, index); }

    void GL::DisableVertexAttribArray(std::uint32_t index) { NEB_GLCall(glDisableVertexAttribArray, index); }

    void GL::VertexAttribPointer(
        std::uint32_t index, std::int32_t size, Type type, bool normalized, sizei_t stride, const void* pointer)
    {
        NEB_GLCall(glVertexAttribPointer, index, size, static_cast<std::uint32_t>(type), normalized, stride, pointer);
    }

    void
        GL::VertexAttribLPointer(std::uint32_t index, std::int32_t size, Type type, sizei_t stride, const void* pointer)
    {
        NEB_GLCall(glVertexAttribLPointer, index, size, static_cast<std::uint32_t>(type), stride, pointer);
    }

    void GL::VertexAttribDivisor(std::uint32_t index, std::uint32_t divisor)
    {
        NEB_GLCall(glVertexAttribDivisor, index, divisor);
    }

    Shader_t GL::CreateShader(ShaderType type) { return NEB_GLCallR(glCreateShader, static_cast<std::uint32_t>(type)); }

    void GL::DeleteShader(Shader_t shader) { NEB_GLCall(glDeleteShader, shader); }

    void GL::ShaderSource(Shader_t shader, sizei_t count, const char** string, const std::int32_t* length)
    {
        NEB_GLCall(glShaderSource, shader, count, string, length);
    }

    void GL::CompileShader(Shader_t shader) { NEB_GLCall(glCompileShader, shader); }

    void GL::GetShaderiv(Shader_t shader, ShaderParameter pname, std::int32_t* params)
    {
        NEB_GLCall(glGetShaderiv, shader, static_cast<std::uint32_t>(pname), params);
    }

    void GL::GetShaderInfoLog(Shader_t shader, sizei_t maxLength, sizei_t* length, char* infoLog)
    {
        NEB_GLCall(glGetShaderInfoLog, shader, maxLength, length, infoLog);
    }

    void GL::GetShaderSource(Shader_t shader, sizei_t maxLength, sizei_t* length, char* source)
    {
        NEB_GLCall(glGetShaderSource, shader, maxLength, length, source);
    }

    Program_t GL::CreateProgram() { return NEB_GLCallR(glCreateProgram); }

    void GL::DeleteProgram(Program_t program) { NEB_GLCall(glDeleteProgram, program); }

    void GL::GetProgramiv(Program_t program, ProgramParameter pname, std::int32_t* params)
    {
        NEB_GLCall(glGetProgramiv, program, static_cast<std::uint32_t>(pname), params);
    }

    void GL::GetProgramInfoLog(Program_t program, std::int32_t maxLength, std::int32_t* length, char* infoLog)
    {
        NEB_GLCall(glGetProgramInfoLog, program, maxLength, length, infoLog);
    }

    void GL::AttachShader(Program_t program, Shader_t shader) { NEB_GLCall(glAttachShader, program, shader); }

    void GL::DetachShader(Program_t program, Shader_t shader) { NEB_GLCall(glDetachShader, program, shader); }

    void GL::LinkProgram(Program_t program) { NEB_GLCall(glLinkProgram, program); }

    void GL::UseProgram(Program_t program)
    {
        if (program == s_BoundProgram)
        {
            return;
        }
        NEB_GLCall(glUseProgram, program);
        s_BoundProgram = program;
    }

    std::int32_t GL::GetUniformLocation(Program_t program, const char* name)
    {
        return NEB_GLCallR(glGetUniformLocation, program, name);
    }

    void GL::Uniform1i(std::int32_t location, std::int32_t value0) { NEB_GLCall(glUniform1i, location, value0); }

    void GL::Uniform1f(std::int32_t location, float value0) { NEB_GLCall(glUniform1f, location, value0); }

    void GL::Uniform2f(std::int32_t location, float value0, float value1)
    {
        NEB_GLCall(glUniform2f, location, value0, value1);
    }

    void GL::Uniform3f(std::int32_t location, float value0, float value1, float value2)
    {
        NEB_GLCall(glUniform3f, location, value0, value1, value2);
    }

    void GL::Uniform4f(std::int32_t location, float value0, float value1, float value2, float value3)
    {
        NEB_GLCall(glUniform4f, location, value0, value1, value2, value3);
    }

    void GL::UniformMatrix3fv(std::int32_t location, std::int32_t count, bool transpose, const float* value)
    {
        NEB_GLCall(glUniformMatrix3fv, location, count, transpose, value);
    }

    void GL::UniformMatrix4fv(std::int32_t location, std::int32_t count, bool transpose, const float* value)
    {
        NEB_GLCall(glUniformMatrix4fv, location, count, transpose, value);
    }

    void GL::GetUniformfv(Program_t program, std::int32_t location, float* params)
    {
        NEB_GLCall(glGetUniformfv, program, location, params);
    }

    void GL::GenTextures(sizei_t n, Texture_t* textures) { NEB_GLCall(glGenTextures, n, textures); }

    void GL::DeleteTextures(sizei_t n, const Texture_t* textures) { NEB_GLCall(glDeleteTextures, n, textures); }

    void GL::BindTexture(TextureTarget target, std::uint32_t texture)
    {
        NEB_GLCall(glBindTexture, static_cast<std::uint32_t>(target), texture);
    }

    void GL::TexImage2D(TextureTarget target,
                        std::int32_t level,
                        InternalFormat internalFormat,
                        std::int32_t width,
                        std::int32_t height,
                        std::int32_t border,
                        PixelFormat format,
                        PixelType type,
                        const void* data)
    {
        NEB_GLCall(glTexImage2D, static_cast<std::uint32_t>(target), level, static_cast<std::uint32_t>(internalFormat),
                   width, height, border, static_cast<std::uint32_t>(format), static_cast<std::uint32_t>(type), data);
    }

    void GL::ActiveTexture(TextureUnit unit) { NEB_GLCall(glActiveTexture, static_cast<std::uint32_t>(unit)); }

    void GL::TexParameteri(TextureTarget target, TextureParameter pname, std::int32_t param)
    {
        NEB_GLCall(glTexParameteri, static_cast<std::uint32_t>(target), static_cast<std::uint32_t>(pname), param);
    }

    Program_t GL::GetBoundProgram() { return s_BoundProgram; }

    VertexArray_t GL::GetBoundVertexArray() { return s_BoundVertexArray; }

    void GL::SetLogLevel(std::uint32_t logLevel) { s_LogLevel = logLevel; }

    void GL::CheckError(const std::string& function, const std::string& file, std::uint32_t line)
    {
        if (s_IsUsingDebugCallback)
        {
            s_LastFunction = GLFunctionCall(function, file, line);
            return;
        }

        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            NEB_CORE_LOG_ERROR("[GL] Error in function {} at line {}: {}", function, line, error);
            s_LastFunction = GLFunctionCall(function, file, line);
        }
    }

    void GL::NebulaGLCallback(std::uint32_t source,
                              std::uint32_t type,
                              std::uint32_t msgId,
                              std::uint32_t severity,
                              sizei_t msg_length,
                              const char* message,
                              [[maybe_unused]] const void* userParam)
    {
        std::uint32_t severity_int = 0;
        std::string severity_str;
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            severity_int = 3;
            severity_str = "HIGH";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            severity_int = 2;
            severity_str = "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            severity_int = 1;
            severity_str = "LOW";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            severity_str = "NOTIFICATION";
            break;
        default:
            severity_str = "UNKNOWN";
            break;
        }

        if (severity_int < s_LogLevel)
        {
            return; // Only log if severity is high enough
        }
        std::string source_str;
        switch (source)
        {
        case GL_DEBUG_SOURCE_API:
            source_str = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            source_str = "WINDOW_SYSTEM";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            source_str = "SHADER_COMPILER";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            source_str = "THIRD_PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            source_str = "APPLICATION";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            source_str = "OTHER";
            break;
        default:
            source_str = "UNKNOWN";
            break;
        }

        std::string type_str;
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            type_str = "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            type_str = "DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            type_str = "UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            type_str = "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            type_str = "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_MARKER:
            type_str = "MARKER";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            type_str = "PUSH_GROUP";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            type_str = "POP_GROUP";
            break;
        case GL_DEBUG_TYPE_OTHER:
            type_str = "OTHER";
            break;
        default:
            type_str = "UNKNOWN";
            break;
        }

        if (msg_length == 0)
        {
            message = "(Empty Message)";
        }
        else if (msg_length == -1)
        {
            message = "(Message Truncated)";
        }

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            NEB_CORE_LOG_ERROR("[OpenGL Error] Source: {0}, Type: {1}, Id: {2}, Severity: {3}, Message: {4}", source_str,
                          type_str, msgId, severity_str, message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            NEB_CORE_LOG_WARN("[OpenGL Error] Source: {0}, Type: {1}, Id: {2}, Severity: {3}, Message: {4}", source_str,
                         type_str, msgId, severity_str, message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            NEB_CORE_LOG_INFO("[OpenGL Error] Source: {0}, Type: {1}, Id: {2}, Severity: {3}, Message: {4}", source_str,
                         type_str, msgId, severity_str, message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            NEB_CORE_LOG_TRACE("[OpenGL Error] Source: {0}, Type: {1}, Id: {2}, Severity: {3}, Message: {4}", source_str,
                          type_str, msgId, severity_str, message);
            break;
        default:
            break;
        }
        NEB_CORE_LOG_DEBUG("[GL] GLCall: {0}, at {1}:{2}", s_LastFunction.Function, s_LastFunction.File,
                      s_LastFunction.Line);

        if (type == GL_DEBUG_TYPE_PERFORMANCE)
        {
            return;
        }
        if (severity_int > 0)
        {
            NEB_DEBUGBREAK();
        }
    }
} // namespace Nebula::OpenGL

