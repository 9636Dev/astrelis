#include "VertexBuffer.hpp"

#ifdef NEBULA_RENDERER_OPENGL
#include "Platform/OpenGL/GLVertexBuffer.hpp"
#endif
#include "RendererAPI.hpp"

namespace Nebula
{
    Ptr<VertexBuffer> VertexBuffer::Create(std::uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            NEBULA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            #ifdef NEBULA_RENDERER_OPENGL
            return MakePtr<OpenGL::GLVertexBuffer>(size).Cast<VertexBuffer>();
            #else
            NEBULA_CORE_ASSERT(false, "RendererAPI::OpenGL is not supported!");
            return nullptr;
            #endif
        default:
            NEBULA_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        };
    }

    Ptr<VertexBuffer> VertexBuffer::Create(float* vertices, std::uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            NEBULA_CORE_ASSERT(false, "Renderer None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            #ifdef NEBULA_RENDERER_OPENGL
            return MakePtr<OpenGL::GLVertexBuffer>(vertices, size).Cast<VertexBuffer>();
            #else
            NEBULA_CORE_ASSERT(false, "Renderer OpenGL is not supported!");
            return nullptr;
            #endif
        default:
            NEBULA_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        };
    }
} // namespace Nebula
