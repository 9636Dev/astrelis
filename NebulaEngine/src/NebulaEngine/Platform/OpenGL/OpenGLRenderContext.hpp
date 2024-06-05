#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Renderer/RenderContext.hpp"

struct GLFWwindow;

namespace Nebula
{
    class OpenGLRenderContext : public RenderContext
    {
    public:
        explicit OpenGLRenderContext(GLFWwindow* windowHandle);
        ~OpenGLRenderContext() override = default;
        OpenGLRenderContext(const OpenGLRenderContext&) = delete;
        OpenGLRenderContext& operator=(const OpenGLRenderContext&) = delete;
        OpenGLRenderContext(OpenGLRenderContext&&) = delete;
        OpenGLRenderContext& operator=(OpenGLRenderContext&&) = delete;

        void Init() override;
        void SwapBuffers() override;

        static Ptr<OpenGLRenderContext> Create(GLFWwindow* window);
    private:
        GLFWwindow* m_WindowHandle;
    };
} // namespace Nebula
