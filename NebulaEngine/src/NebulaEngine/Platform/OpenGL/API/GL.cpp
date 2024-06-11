#include "GL.hpp"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "NebulaEngine/Core/Log.hpp"
#include "NebulaEngine/Core/Utils/Function.hpp"

#ifdef NEBULA_DEBUG
    #define GLCall(x)                                                   \
        switch (0)                                                      \
        {                                                               \
        default:                                                        \
            ::Nebula::OpenGL::GL::GLCallHelper(__FILE__, __LINE__, #x); \
            x;                                                          \
        }
    #define GLCallR(x)                                                  \
        [&]() {                                                         \
            ::Nebula::OpenGL::GL::GLCallHelper(__FILE__, __LINE__, #x); \
            return x;                                                   \
        }()
#else
    #define GLCall(x)  x
    #define GLCAllR(x) x
#endif

namespace Nebula::OpenGL
{
    InitInfo GL::Init()
    {
        int res = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        if (res == 0)
        {
            return {GLVersion(0, 0), false};
        }

        // GLAD successfully loaded
        GLVersion version = GetVersion();
        if (version.Major >= 4 && version.Minor >= 3)
        {
            EnableDebugCallback();
            return {version, true};
        }

        return {version, false};
    }

    void GL::Clear(ClearTarget target)
    {
        GLCall(glClear(static_cast<std::uint32_t>(target)));
    }

    void GL::Viewport(int posX, int posY, int width, int height)
    {
        GLCall(glViewport(posX, posY, width, height));
    }

    void GL::DrawElements(DrawMode mode, std::size_t count, GLType type, const void* indices)
    {
        GLCall(glDrawElements(static_cast<std::uint32_t>(mode), count, static_cast<std::uint32_t>(type), indices));
    }

    void GL::DrawElementsInstanced(DrawMode mode, std::size_t count, GLType type, const void* indices, std::uint32_t primcount)
    {
        GLCall(glDrawElementsInstanced(static_cast<std::uint32_t>(mode), count, static_cast<std::uint32_t>(type), indices, primcount));
    }

    void GL::GenVertexArrays(std::uint32_t n, std::uint32_t* arrays)
    {
        GLCall(glGenVertexArrays(n, arrays));
    }

    void GL::BindVertexArray(std::uint32_t array)
    {
        GLCall(glBindVertexArray(array));
    }

    void GL::DeleteVertexArrays(std::uint32_t n, const std::uint32_t* arrays)
    {
        GLCall(glDeleteVertexArrays(n, arrays));
    }

    void GL::EnableVertexAttribArray(std::uint32_t index)
    {
        GLCall(glEnableVertexAttribArray(index));
    }

    void GL::VertexAttribPointer(std::uint32_t index, std::uint32_t size, GLType type, bool normalized, std::uint32_t stride, const void* pointer)
    {
        GLCall(glVertexAttribPointer(index, size, static_cast<std::uint32_t>(type), normalized, stride, pointer));
    }

    void GL::VertexAttribDivisor(std::uint32_t index, std::uint32_t divisor)
    {
        GLCall(glVertexAttribDivisor(index, divisor));
    }

    void GL::GenBuffers(std::uint32_t n, std::uint32_t* buffers)
    {
        GLCall(glGenBuffers(n, buffers));
    }

    void GL::BindBuffer(BufferTarget target, std::uint32_t buffer)
    {
        GLCall(glBindBuffer(static_cast<std::uint32_t>(target), buffer));
    }

    void GL::BufferData(BufferTarget target, std::size_t size, const void* data, BufferUsage usage)
    {
        GLCall(glBufferData(static_cast<std::uint32_t>(target), size, data, static_cast<std::uint32_t>(usage)));
    }

    void GL::DeleteBuffers(std::uint32_t n, const std::uint32_t* buffers)
    {
        GLCall(glDeleteBuffers(n, buffers));
    }

    std::uint32_t GL::CreateShader(ShaderType type)
    {
        return GLCallR(glCreateShader(static_cast<std::uint32_t>(type)));
    }

    void GL::ShaderSource(std::uint32_t shader, std::uint32_t count, const char** string, const int* length)
    {
        GLCall(glShaderSource(shader, count, string, length));
    }

    void GL::CompileShader(std::uint32_t shader)
    {
        GLCall(glCompileShader(shader));
    }

    void GL::GetShaderiv(std::uint32_t shader, ShaderParameter pname, int* params)
    {
        GLCall(glGetShaderiv(shader, static_cast<std::uint32_t>(pname), params));
    }

    void GL::GetShaderInfoLog(std::uint32_t shader, int maxLength, int* length, char* infoLog)
    {
        GLCall(glGetShaderInfoLog(shader, maxLength, length, infoLog));
    }

    void GL::DeleteShader(std::uint32_t shader)
    {
        GLCall(glDeleteShader(shader));
    }

    std::uint32_t GL::CreateProgram()
    {
        return GLCallR(glCreateProgram());
    }

    void GL::AttachShader(std::uint32_t program, std::uint32_t shader)
    {
        GLCall(glAttachShader(program, shader));
    }

    void GL::LinkProgram(std::uint32_t program)
    {
        GLCall(glLinkProgram(program));
    }

    void GL::GetProgramiv(std::uint32_t program, ProgramParameter pname, int* params)
    {
        GLCall(glGetProgramiv(program, static_cast<std::uint32_t>(pname), params));
    }

    void GL::GetProgramInfoLog(std::uint32_t program, int maxLength, int* length, char* infoLog)
    {
        GLCall(glGetProgramInfoLog(program, maxLength, length, infoLog));
    }

    void GL::UseProgram(std::uint32_t program)
    {
        GLCall(glUseProgram(program));
    }

    void GL::DeleteProgram(std::uint32_t program)
    {
        GLCall(glDeleteProgram(program));
    }

    GLVersion GL::GetVersion()
    {
        static GLVersion version(0, 0);

        if (version.Major != 0)
        {
            return version;
        }
        glGetIntegerv(GL_MAJOR_VERSION, &version.Major);
        glGetIntegerv(GL_MINOR_VERSION, &version.Minor);

        return version;
    }

    void GL::EnableDebugCallback()
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GLDebugMessageCallback, nullptr);
        s_DebugCallback = true;
    }

    void GL::GLCallHelper(std::string_view file, int line, std::string_view function)
    {
        if (s_DebugCallback)
        {
            s_CallLocation = {file, line, function};
            return;
        }

        GLenum error = 0;
        while ((error = glGetError()) != GL_NO_ERROR)
        {
            std::string errorString;
            switch (error)
            {
            case GL_INVALID_ENUM:
                errorString = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                errorString = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                errorString = "GL_INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                errorString = "GL_STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                errorString = "GL_STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                errorString = "GL_OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            default:
                errorString = "UNKNOWN";
                break;
            }

            NEBULA_CORE_LOG_ERROR("OpenGL Error: {0} in {1} at line {2} in function {3}", errorString, file, line, function);
        }
    }

    void GL::GLDebugMessageCallback(std::uint32_t source,
                                    std::uint32_t type,
                                    std::uint32_t errId,
                                    std::uint32_t severity,
                                    int length,
                                    const char* message,
                                    const void* userParam)
    {
        NEBULA_UNUSED(userParam);
        NEBULA_UNUSED(length);
        std::string sourceString;
        switch (source)
        {
        case GL_DEBUG_SOURCE_API:
            sourceString = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            sourceString = "Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            sourceString = "Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            sourceString = "Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            sourceString = "Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            sourceString = "Other";
            break;
        default:
            sourceString = "UNKNOWN";
            break;
        }

        std::string typeString;
        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            typeString = "Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            typeString = "Deprecated Behavior";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            typeString = "Undefined Behavior";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            typeString = "Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            typeString = "Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            typeString = "Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            typeString = "Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            typeString = "Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            typeString = "Other";
            break;
        default:
            typeString = "UNKNOWN";
            break;
        }

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            NEBULA_CORE_LOG_FATAL("[OpenGL High Severity Error] {0} ({1}) at {2}:{3} ({4}) [{5} {6}]", message, errId, s_CallLocation.File, s_CallLocation.Line, s_CallLocation.Function, sourceString, typeString);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            NEBULA_CORE_LOG_ERROR("[OpenGL Medium Severity Error] {0} ({1}) at {2}:{3} ({4}) [{5} {6}]", message, errId, s_CallLocation.File, s_CallLocation.Line, s_CallLocation.Function, sourceString, typeString);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            NEBULA_CORE_LOG_WARN("[OpenGL Low Severity Error] {0} ({1}) at {2}:{3} ({4}) [{5} {6}]", message, errId, s_CallLocation.File, s_CallLocation.Line, s_CallLocation.Function, sourceString, typeString);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            if (type == GL_DEBUG_TYPE_PERFORMANCE)
            {
                NEBULA_CORE_LOG_TRACE("[OpenGL Performance Notification] {0}", message);
                break;
            }
            NEBULA_CORE_LOG_INFO("[OpenGL Notification] {0} ({1}) at {2}:{3} ({4}) [{5} {6}]", message, errId, s_CallLocation.File, s_CallLocation.Line, s_CallLocation.Function, sourceString, typeString);
            break;
        default:
            break;
        }
    }

    bool GL::s_DebugCallback = false;
    GL::CallLocation GL::s_CallLocation;
} // namespace Nebula::OpenGL
