#include "OpenGLRenderer.hpp"

#include "NebulaEngine/Core/Utils/Function.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Nebula
{
    OpenGLRenderer::OpenGLRenderer(Ref<Window> window, Bounds bounds) : m_Bounds(bounds)
    {
        // To suppres the warning about using a const reference, in the future we need to store window for changing the context
        auto windowMoved = std::move(window);
        NEBULA_UNUSED(windowMoved);

        glViewport(m_Bounds.X, m_Bounds.Y, m_Bounds.Width, m_Bounds.Height);
    }

    OpenGLRenderer::~OpenGLRenderer() = default;

    void OpenGLRenderer::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    Result<Ptr<OpenGLRenderer>, std::string> OpenGLRenderer::Create(Ref<Window> window, Bounds bounds)
    {
        if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
        {
            return "Failed to initialize Renderer API (GLAD - OpenGL)";
        }

        return MakePtr<OpenGLRenderer>(std::move(window), bounds);
    }
} // namespace Nebula
