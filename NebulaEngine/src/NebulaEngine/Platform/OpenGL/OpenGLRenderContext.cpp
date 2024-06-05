#include "OpenGLRenderContext.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "NebulaEngine/Core/Base.hpp"

namespace Nebula
{
    OpenGLRenderContext::OpenGLRenderContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
    }

    void OpenGLRenderContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        NEBULA_CORE_ASSERT(status, "Failed to initialize Glad!");
    }

    void OpenGLRenderContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}  // namespace Nebula
