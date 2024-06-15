#include "OpenGLRendererAPI.hpp"

#include "API/GL.hpp"

namespace Nebula
{
    void OpenGLRendererAPI::Init()
    {
        auto initInfo = OpenGL::GL::Init();
        NEBULA_CORE_LOG_DEBUG("OpenGL Version: {0}.{1}", initInfo.Version.Major, initInfo.Version.Minor);
    }

    void OpenGLRendererAPI::SetClearColor(const Vector4f &color)
    {
        OpenGL::GL::ClearColor(color.x(), color.y(), color.z(), color.w());
    }

    void OpenGLRendererAPI::Clear()
    {
        OpenGL::GL::Clear(OpenGL::ClearTarget::Color | OpenGL::ClearTarget::Depth | OpenGL::ClearTarget::Stencil);
    }

    void OpenGLRendererAPI::SetViewport(std::int32_t posX, std::int32_t posY, std::int32_t width, std::int32_t height)
    {
        OpenGL::GL::Viewport(posX, posY, width, height);
    }
} // namespace Nebula
