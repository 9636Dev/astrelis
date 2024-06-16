#include "RenderContext.hpp"

#include "RendererAPI.hpp"

#include "NebulaEngine/Core/Assert.hpp"

#ifdef NEBULA_RENDERER_OPENGL
#include "Platform/OpenGL/OpenGLContext.hpp"
#endif

namespace Nebula
{
    Ptr<RenderContext> RenderContext::Create(void* window)
    {
        switch (RendererAPI::GetAPI())
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
