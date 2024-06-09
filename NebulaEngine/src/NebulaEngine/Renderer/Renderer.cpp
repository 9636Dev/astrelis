#include "Renderer.hpp"

#include <utility>

#include "NebulaEngine/Platform/OpenGL/OpenGLRenderer.hpp"

namespace Nebula
{
    RendererAPI Renderer::s_RendererAPI = RendererAPI::OpenGL;

    Result<Ptr<Renderer>, std::string> Renderer::Create(Ref<Window> window, Bounds bounds)
    {
        switch (s_RendererAPI)
        {
        case RendererAPI::OpenGL:
            return OpenGLRenderer::Create(std::move(window), bounds).MapMove([](Ptr<OpenGLRenderer>&& renderer) {
                return renderer.Cast<Renderer>();
            });
        case RendererAPI::None:
            return "RendererAPI::None is not supported";
        default:
            return "Unknown RendererAPI";
        }
    }
} // namespace Nebula
