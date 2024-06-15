#include "OpenGLContext.hpp"

#include "NebulaEngine/Core/Base.hpp"

namespace Nebula
{
    OpenGLRenderContext::OpenGLRenderContext(GLFWwindow* window)
        : m_Window(window)
    {
    }

    OpenGLRenderContext::~OpenGLRenderContext()
    = default;

    void OpenGLRenderContext::Init()
    {
        glfwMakeContextCurrent(m_Window);
    }

    void OpenGLRenderContext::SwapBuffers()
    {
        glfwSwapBuffers(m_Window);
    }

    Ptr<OpenGLRenderContext> OpenGLRenderContext::Create(void* window)
    {
        if (window == nullptr)
        {
            NEBULA_CORE_LOG_ERROR("Window is null!");
            return Ptr<OpenGLRenderContext>(nullptr);
        }
        return MakePtr<OpenGLRenderContext>(static_cast<GLFWwindow*>(window));
    }
}
