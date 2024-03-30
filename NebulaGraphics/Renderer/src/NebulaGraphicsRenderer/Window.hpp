#pragma once

#include "Core.hpp"

#include <NebulaGraphicsCore/Window.hpp>
#include <string>

namespace Nebula
{
    NEBULA_GRAPHICS_OPENGL_API Window* CreateGLFWWindow(const std::string& libraryPath);
    NEBULA_GRAPHICS_OPENGL_API bool DestroyGLFWWindow(Window* window);
} // namespace Nebula
