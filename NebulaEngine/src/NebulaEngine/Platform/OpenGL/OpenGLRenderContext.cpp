#include "OpenGLRenderContext.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "NebulaEngine/Core/Base.hpp"

#include "API/GL.hpp"

namespace Nebula
{
    OpenGLRenderContext::OpenGLRenderContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
    }

    void OpenGLRenderContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        auto res = Nebula::OpenGL::GL::Init();
        NEBULA_CORE_VERIFY(res.Version.Major != 0, "Failed to initialize OpenGL");
        NEBULA_CORE_LOG_INFO("OpenGL Version: {0}.{1}", res.Version.Major, res.Version.Minor);
        NEBULA_CORE_LOG_INFO("OpenGL Debug Callback: {0}", res.DebugCallback);
    }

    void OpenGLRenderContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}  // namespace Nebula
