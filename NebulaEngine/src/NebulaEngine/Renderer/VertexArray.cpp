#include "VertexArray.hpp"

#include "NebulaEngine/Core/Base.hpp"

#include "RendererAPI.hpp"
#ifdef NEBULA_RENDERER_OPENGL
    #include "Platform/OpenGL/GLVertexArray.hpp"
#endif


namespace Nebula
{
    Ptr<VertexArray> VertexArray::Create()
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            NEBULA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return MakePtr<OpenGL::GLVertexArray>().Cast<VertexArray>();
        default:
            NEBULA_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        }
    }
} // namespace Nebula
