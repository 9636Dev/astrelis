#include "RenderContext.hpp"

#include "NebulaEngine/Platform/OpenGL/OpenGLRenderContext.hpp"

namespace Nebula
{
    Ptr<RenderContext> RenderContext::Create(void* window)
    {
        return MakePtr<OpenGLRenderContext>(static_cast<GLFWwindow*>(window)).Cast<RenderContext>();
    }
} // namespace Nebula
