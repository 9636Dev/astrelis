#include "Shader.hpp"

#include "NebulaEngine/Core/Assert.hpp"
#include "NebulaEngine/Core/Utils/Function.hpp"
#include "NebulaEngine/Renderer/RendererAPI.hpp"

#ifdef NEBULA_RENDERER_OPENGL
    #include "Platform/OpenGL/GLShader.hpp"
#endif

namespace Nebula
{
    Ptr<Shader> Shader::Create(std::string name, std::string_view vertexSrc, std::string_view fragmentSrc)
    {
        NEBULA_UNUSED(name);
        NEBULA_UNUSED(vertexSrc);
        NEBULA_UNUSED(fragmentSrc);
        switch (RendererAPI::GetAPI())
        {
        case RendererAPI::API::None:
            NEBULA_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
            return nullptr;
        case RendererAPI::API::OpenGL:
#ifdef NEBULA_RENDERER_OPENGL
            return OpenGL::GLShaderProgram::Create(std::move(name), vertexSrc, fragmentSrc).Cast<Shader>();
#else
            NEBULA_CORE_ASSERT(false, "RendererAPI::OpenGL is not supported");
            return nullptr;
#endif
        }
    }
} // namespace Nebula
