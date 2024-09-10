#pragma once

#include "Astrelis/Renderer/RenderContext.hpp"

#include <GLFW/glfw3.h>

namespace Astrelis
{
    class OpenGLRenderContext : public RenderContext
    {
    public:
        explicit OpenGLRenderContext(GLFWwindow* window);
        ~OpenGLRenderContext() override;
        OpenGLRenderContext(const OpenGLRenderContext&) = delete;
        OpenGLRenderContext& operator=(const OpenGLRenderContext&) = delete;
        OpenGLRenderContext(OpenGLRenderContext&&) = delete;
        OpenGLRenderContext& operator=(OpenGLRenderContext&&) = delete;

        void Init() override;
        void SwapBuffers() override;

        static Ptr<OpenGLRenderContext> Create(void* window);
    private:
        GLFWwindow* m_Window;
    };
} // namespace Astrelis
