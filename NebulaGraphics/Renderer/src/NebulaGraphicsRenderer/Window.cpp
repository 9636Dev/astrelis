#include "Window.hpp"

// TODO: Make cross-platform
#include <NebulaCore/Log.hpp>

#include <Windows.h>


namespace Nebula
{

    // TODO(9636D): Keep track of loaded libraries and unload them when the application closes

    Window* CreateGLFWWindow(const std::string& libraryPath)
    {
        auto* library = LoadLibrary(libraryPath.c_str());
        if (library == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to load library: {0}", libraryPath);
            return nullptr;
        }

        using CreateGLFWWindowFunc = Window* (*)();        
        auto* createGLFWWindow = reinterpret_cast<CreateGLFWWindowFunc>(GetProcAddress(library, "InternalCreateGLFWWindow")); 

        if (createGLFWWindow == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to load symbol: InternalCreateGLFWWindow");
            return nullptr;
        }

        return createGLFWWindow();
    }

    bool DestroyGLFWWindow(Window* window)
    {
        delete window;
        return true;
    }
} // namespace Nebula
