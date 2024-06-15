#include "RenderContext.hpp"

#include "Renderer.hpp"

#include "NebulaEngine/Core/Assert.hpp"

#include "Platform/OpenGL/OpenGLContext.hpp"

namespace Nebula
{
    Ptr<RenderContext> RenderContext::Create(void* window)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            NEBULA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
#ifdef NEBULA_RENDERER_OPENGL
            return OpenGLRenderContext::Create(window).Cast<RenderContext>();
#else
            NEBULA_CORE_ASSERT(false, "RendererAPI::API::OpenGL is currently not supported!");
            return nullptr;

#endif
        default:
            NEBULA_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }
} // namespace Nebula
