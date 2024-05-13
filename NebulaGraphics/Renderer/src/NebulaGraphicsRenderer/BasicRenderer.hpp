#pragma once

#include "Core.hpp"

#include "NebulaCore/Util.hpp"
#include "NebulaGraphicsCore/Basic/Renderer.hpp"
#include "NebulaGraphicsCore/Renderer.hpp"
#include "NebulaGraphicsCore/Window.hpp"

#include <string>

namespace Nebula
{
    NEBULA_GRAPHICS_OPENGL_API Basic::RendererCreationResult CreateBasicGLRenderer(std::string libraryPath,
                                                                       const std::shared_ptr<Window>& window,
                                                                       const OpenGLContext& context);
    NEBULA_GRAPHICS_OPENGL_API Basic::RendererCreationResult CreateBasicMetalRenderer(std::string libraryPath,
                                                                          const std::shared_ptr<Window>& window,
                                                                          const MetalContext& context);
    NEBULA_GRAPHICS_OPENGL_API bool DestroyBasicRenderer(std::shared_ptr<Basic::Renderer>& window);

    template<typename T>
        requires std::is_base_of_v<RenderFrameworkContext, T>
    Basic::RendererCreationResult
        CreateBasicRenderer(const std::string& libraryPath, const std::shared_ptr<Window>& window, const T& context)
    {
        if constexpr (std::is_same_v<T, OpenGLContext>)
        {
            return CreateBasicGLRenderer(libraryPath + NEBULA_PLATFORM_SHARED_LIBRARY_EXTENSION, window, context);
        }
        if constexpr (std::is_same_v<T, MetalContext>)
        {
            //return CreateBasicMetalRenderer(libraryPath + NEBULA_PLATFORM_SHARED_LIBRARY_EXTENSION, window, context);
            return {};
        }
    }

} // namespace Nebula
