#include "OpenGLRenderer.hpp"
#include "NebulaRenderer/Renderer.hpp"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <utility>

namespace Nebula
{
    OpenGLRenderer::OpenGLRenderer(Ref<Window> window) : m_Window(std::move(window)) {}

    OpenGLRenderer::~OpenGLRenderer() = default;

    void OpenGLRenderer::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    Result<Ptr<Renderer>, RendererError> CreateRenderer(Ref<Window> window)
    {
        auto* glfwWindow = static_cast<GLFWwindow*>(window->GetNativeWindow());

        if (glfwWindow == nullptr)
        {
            return RendererError::FailedToAttach;
        }
        glfwMakeContextCurrent(glfwWindow);

        if (glewInit() != GLEW_OK)
        {
            return RendererError::FailedToInit;
        }

        return MakePtr<OpenGLRenderer>(std::move(window));
    }
} // namespace Nebula
