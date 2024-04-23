#pragma once

#include "Core.hpp"

#include "NebulaCore/Util.hpp"
#include "NebulaGraphicsCore/Renderer.hpp"
#include "NebulaGraphicsCore/Window.hpp"

#include <string>

namespace Nebula
{
    NEBULA_GRAPHICS_OPENGL_API RendererCreationResult CreateGLRenderer(std::string libraryPath,
                                                                       const std::shared_ptr<Window>& window,
                                                                       const OpenGLContext& context);
    NEBULA_GRAPHICS_OPENGL_API RendererCreationResult CreateMetalRenderer(std::string libraryPath,
                                                                          const std::shared_ptr<Window>& window,
                                                                          const MetalContext& context);
    NEBULA_GRAPHICS_OPENGL_API bool DestroyRenderer(std::shared_ptr<Renderer>& window);

    template<typename T>
        requires std::is_base_of_v<RenderFrameworkContext, T>
    RendererCreationResult CreateRenderer(const std::string& libraryPath, const std::shared_ptr<Window>& window, const T& context)
    {
        if constexpr (std::is_same_v<T, OpenGLContext>)
        {
            return CreateGLRenderer(libraryPath + NEBULA_PLATFORM_SHARED_LIBRARY_EXTENSION, window, context);
        }
        if constexpr (std::is_same_v<T, MetalContext>)
        {
            return CreateMetalRenderer(libraryPath + NEBULA_PLATFORM_SHARED_LIBRARY_EXTENSION, window, context);
        }
    }

} // namespace Nebula
