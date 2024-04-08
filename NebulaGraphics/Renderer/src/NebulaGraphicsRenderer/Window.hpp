#pragma once

#include "Core.hpp"
#include "NebulaCore/Util.hpp"

#include <NebulaGraphicsCore/Window.hpp>
#include <string>

namespace Nebula
{
    NEBULA_GRAPHICS_OPENGL_API WindowCreationResult CreateGLFWWindow(std::string libraryPath,
                                                                     WindowProps<OpenGLContext>& props);
    NEBULA_GRAPHICS_OPENGL_API bool DestroyWindow(std::shared_ptr<Window>& window);

    template<typename T>
        requires std::is_base_of_v<RenderFrameworkContext, T>
    WindowCreationResult CreateWindow(const std::string& libraryPath, WindowProps<T>& props)
    {
        if constexpr (std::is_same_v<T, OpenGLContext>)
        {
            return CreateGLFWWindow(libraryPath + NEBULA_PLATFORM_SHARED_LIBRARY_EXTENSION, props);
        }
    }

} // namespace Nebula
