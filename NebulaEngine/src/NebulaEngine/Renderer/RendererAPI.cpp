#include "RendererAPI.hpp"

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Core/Pointer.hpp"

#ifdef NEBULA_RENDERER_OPENGL
    #include "Platform/OpenGL/OpenGLRendererAPI.hpp"
#endif

namespace Nebula
{
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    Ptr<RendererAPI> RendererAPI::Create() noexcept
    {
        switch (s_API)
        {
        case RendererAPI::API::None:
            NEBULA_CORE_ASSERT(false, "RendererAPI::API::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
#ifdef NEBULA_RENDERER_OPENGL
            return MakePtr<OpenGLRendererAPI>().Cast<RendererAPI>();
#else
            NEBULA_CORE_ASSERT(false, "RendererAPI::API::OpenGL is currently not supported!");
            return nullptr;
#endif

        default:
            return nullptr;
        }
    }
} // namespace Nebula
