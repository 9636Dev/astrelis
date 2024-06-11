#include "RenderContext.hpp"

#include "Renderer.hpp"

#include "NebulaEngine/Core/Base.hpp"
#include "NebulaEngine/Platform/OpenGL/OpenGLRenderContext.hpp"

namespace Nebula
{
    Ptr<RenderContext> RenderContext::Create(void* window)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            NEBULA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return Ptr<RenderContext>(nullptr);
        case RendererAPI::OpenGL:
            return OpenGLRenderContext::Create(static_cast<GLFWwindow*>(window)).Cast<RenderContext>();
        default:
            NEBULA_CORE_ASSERT(false, "Unknown RendererAPI!");
            return Ptr<RenderContext>(nullptr);
        }
    }
} // namespace Nebula
