#include "Renderer.hpp"

#include "NebulaGraphicsOpenGL/Core.hpp"
#include "OpenGL/GL.hpp"
#include "Window.hpp"

namespace Nebula
{
    OpenGLRenderer::OpenGLRenderer()
    {

        s_RendererCount++;


    }

    OpenGLRenderer::~OpenGLRenderer()
    {
        if (--s_RendererCount == 0)
        {
            OpenGL::GL::Terminate();
        }

    }

    void OpenGLRenderer::Render()
    {
        // We are just using this function to test the renderer at the moment
        OpenGL::GL::Clear(OpenGL::ClearTarget::ColorBufferBit);

    }
} // namespace Nebula
//
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern "C" NEBULA_GRAPHICS_OPENGL_API Nebula::RendererCreationResult
    nebulaGraphicsOpenGLCreateOpenGLRenderer(const Nebula::OpenGLContext& context,
                                             const std::shared_ptr<Nebula::Window>& window)
{
    std::shared_ptr<Nebula::GLFWWindow> windowPtr = std::dynamic_pointer_cast<Nebula::GLFWWindow>(window);
    if (context.MajorVersion == 0 || windowPtr == nullptr)
    {
        return {nullptr, Nebula::RendererCreationResult::ErrorType::InvalidArguments};
    }

    windowPtr->MakeContextCurrent();
    Nebula::OpenGL::GLVersion version = {context.MajorVersion, context.MinorVersion};
    Nebula::OpenGL::GL::Init(version);

    if (version.Major == 0)
    {
        return {nullptr, Nebula::RendererCreationResult::ErrorType::RendererInitFailed};
    }

    auto renderer = std::make_shared<Nebula::OpenGLRenderer>();
    return {renderer, Nebula::RendererCreationResult::ErrorType::None};
}
#pragma clang diagnostic pop
