#include "GL.hpp"

#include <GL/glew.h>

#include "NebulaCore/Log.hpp"
#include "NebulaCore/Util/Assert.hpp"
#include "NebulaCore/Util/Debug.hpp"

namespace Nebula::OpenGL
{
    bool GL::s_Initialized = false;
    bool GL::s_DebugOutput = false;
    GL::FunctionCall GL::s_CurrentFunction;

#ifdef NEBULA_DEBUG
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define NEB_GLCall(func, ...)                                        \
        switch (0)                                                       \
        {                                                                \
        default:                                                         \
            func(__VA_ARGS__);                                           \
            ::Nebula::OpenGL::GL::CheckError(#func, __FILE__, __LINE__); \
        }
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define NEB_GLCallR(func, ...) [&]() { auto result = func(__VA_ARGS__); ::Nebula::OpenGL::GL::CheckError(#func, __FILE__, __LINE__);
#else
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define NEB_GLCall(func, ...)  func(__VA_ARGS__)
    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define NEB_GLCallR(func, ...) func(__VA_ARGS__)
#endif

    bool GL::Init()
    {
        if (s_Initialized)
        {
            return true;
        }

        if (glewInit() != GLEW_OK)
        {
            return false;
        }

        s_Initialized = true;
        NEBULA_CORE_LOG_INFO("[GL] Initialized");

#ifdef NEBULA_DEBUG
        std::int32_t majorVersion = 0;
        GetIntegerv(GetTarget::MajorVersion, majorVersion);

        std::int32_t minorVersion = 0;
        GetIntegerv(GetTarget::MinorVersion, minorVersion);

        s_DebugOutput = (majorVersion > 4) || (majorVersion == 4 && minorVersion >= 3) || GLEW_ARB_debug_output;

        NEBULA_CORE_LOG_INFO("OpenGL Info:");
        const char* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        NEBULA_CORE_LOG_INFO("  Vendor: {0}", vendor);
        const char* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        NEBULA_CORE_LOG_INFO("  Renderer: {0}", renderer);
        NEBULA_CORE_LOG_INFO("  Version: {0}.{1}", majorVersion, minorVersion);
        const char* glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
        NEBULA_CORE_LOG_INFO("  GLSL Version: {0}", glslVersion);
        NEBULA_CORE_LOG_INFO("  Debug Output: {0}", s_DebugOutput ? "4.3 Debug Output" : "No Debug Output");
        NEBULA_CORE_LOG_INFO("  ErrorChecking: {0}", s_DebugOutput ? "Included" : "Explicitly Check Errors");

        if (s_DebugOutput)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(DebugCallback, nullptr);
        }
#endif

        return true;
    }

    void GL::Shutdown() { NEBULA_CORE_LOG_INFO("[GL] Shutting down"); }

    void GL::ClearColor(float red, float green, float blue, float alpha)
    {
        NEBULA_CORE_ASSERT(red >= 0.0F && red <= 1.0F, "Red value must be between 0.0 and 1.0");
        NEBULA_CORE_ASSERT(green >= 0.0F && green <= 1.0F, "Green value must be between 0.0 and 1.0");
        NEBULA_CORE_ASSERT(blue >= 0.0F && blue <= 1.0F, "Blue value must be between 0.0 and 1.0");
        NEB_GLCall(glClearColor, red, green, blue, alpha);
    }

    void GL::Clear(ClearBufferMask mask)
    {
        NEBULA_CORE_ASSERT(
            (static_cast<std::uint32_t>(mask) &
             static_cast<std::uint32_t>(ClearBufferMask::ColorBufferBit | ClearBufferMask::DepthBufferBit |
                                        ClearBufferMask::StencilBufferBit)) != 0,
            "Clear mask must include at least one buffer bit");
        NEB_GLCall(glClear, static_cast<std::uint32_t>(mask));
    }

    void GL::GetIntegerv(GetTarget target, int& data) { glGetIntegerv(static_cast<std::uint32_t>(target), &data); }

    void GL::CheckError(const std::string_view& function, const std::string_view& file, std::int32_t line)
    {
        if (s_DebugOutput)
        {
            s_CurrentFunction = {function, file, line};
            return;
        }

        GLenum error = glGetError();
        if (error == GL_NO_ERROR)
        {
            return;
        }

        const char* errorString = reinterpret_cast<const char*>(gluErrorString(error));
        NEBULA_CORE_LOG_ERROR("OpenGL Error: {0} ({1} in {2}:{3})", errorString, function, file, line);
    }

    void GL::DebugCallback(std::uint32_t source,
                           std::uint32_t type,
                           std::uint32_t errorId,
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
            severity_int = 0;
            severity_str = "NOTIFICATION";
            break;
        default:
            severity_str = "UNKNOWN";
            break;
        }

        if (severity_int < 1)
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
            NEBULA_CORE_LOG_ERROR("[OpenGL Error] Source: {0}, Type: {1}, Id: {2}, Severity: {3}, Message: {4}",
                                  source_str, type_str, errorId, severity_str, message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            NEBULA_CORE_LOG_WARN("[OpenGL Error] Source: {0}, Type: {1}, Id: {2}, Severity: {3}, Message: {4}",
                                 source_str, type_str, errorId, severity_str, message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            NEBULA_CORE_LOG_DEBUG("[OpenGL Error] Source: {0}, Type: {1}, Id: {2}, Severity: {3}, Message: {4}",
                                  source_str, type_str, errorId, severity_str, message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            NEBULA_CORE_LOG_TRACE("[OpenGL Error] Source: {0}, Type: {1}, Id: {2}, Severity: {3}, Message: {4}",
                                  source_str, type_str, errorId, severity_str, message);
            break;
        default:
            break;
        }
        NEBULA_CORE_LOG_DEBUG("[OpenGL Error Source] Function: {0}, at {1}:{2}", s_CurrentFunction.Function,
                              s_CurrentFunction.File, s_CurrentFunction.Line);

        if (type == GL_DEBUG_TYPE_PERFORMANCE)
        {
            return;
        }
        if (severity_int > 0)
        {
            NEBULA_DEBUG_BREAK();
        }
    }
} // namespace Nebula::OpenGL
