#pragma once

#include "Core.hpp"

#include <NebulaGraphicsCore/Window.hpp>
#include <string>

namespace Nebula
{
    NEBULA_GRAPHICS_OPENGL_API WindowCreationResult CreateGLFWWindow(std::string libraryPath, WindowProps<OpenGLContext>& props);
    NEBULA_GRAPHICS_OPENGL_API bool DestroyWindow(std::shared_ptr<Window>& window);

    template <typename T>
        requires std::is_base_of_v<RenderFrameworkContext, T>
    WindowCreationResult CreateWindow(std::string libraryPath, WindowProps<T>& props)
    {
        if constexpr (std::is_same_v<T, OpenGLContext>)
        {
            return CreateGLFWWindow(std::move(libraryPath), props);
        }
    }

} // namespace Nebula
