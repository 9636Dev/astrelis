#include "IndexBuffer.hpp"

#include "NebulaEngine/Core/Assert.hpp"

#ifdef NEBULA_RENDERER_OPENGL
#include "Platform/OpenGL/GLIndexBuffer.hpp"
#endif
#include "RendererAPI.hpp"

namespace Nebula
{
    Ptr<IndexBuffer> IndexBuffer::Create(std::uint32_t count)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            NEBULA_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            #ifdef NEBULA_RENDERER_OPENGL
            return MakePtr<OpenGL::GLIndexBuffer>(count).Cast<IndexBuffer>();
            #else
            NEBULA_CORE_ASSERT(false, "RendererAPI::OpenGL is not supported!");
            return nullptr;
            #endif
        default:
            NEBULA_CORE_ASSERT(false, "Unknown RendererAPI!");
            return nullptr;
        };
    }

    Ptr<IndexBuffer> IndexBuffer::Create(const std::uint32_t* data, std::uint32_t size)
    {
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            NEBULA_CORE_ASSERT(false, "Renderer None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            #ifdef NEBULA_RENDERER_OPENGL
            return MakePtr<OpenGL::GLIndexBuffer>(data, size).Cast<IndexBuffer>();
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

