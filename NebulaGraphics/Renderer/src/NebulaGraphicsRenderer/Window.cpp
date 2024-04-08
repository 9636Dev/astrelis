#include "Window.hpp"

#include "Loader.hpp"
#include "NebulaCore/Assert.hpp"
#include "NebulaCore/Log.hpp"

namespace Nebula
{
    WindowCreationResult CreateGLFWWindow(std::string libraryPath, WindowProps<OpenGLContext>& props)
    {
        Loader::ModuleHandle library = Loader::LoadGraphicsLibrary(libraryPath);
        if (library.Handle == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to load library: {0}", libraryPath);
            return {nullptr, WindowCreationResult::ErrorType::LibraryLoadFailed};
        }

        using CreateGLFWWindowFunc = WindowCreationResult (*)(WindowProps<OpenGLContext>&);
        auto* createGLFWWindow =
            /* NOLINT */ reinterpret_cast<CreateGLFWWindowFunc>(
                Loader::LoadSymbol(library, "nebulaGraphicsOpenGLCreateGLFWWindow"));

        if (createGLFWWindow == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to load symbol: nebulaGraphicsOpenGLCreateGLFWWindow");
            return {nullptr, WindowCreationResult::ErrorType::SymbolLoadFailed};
        }

        auto result = createGLFWWindow(props);
        if (result.Window == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to create window");
            return result;
        }
        Loader::InsertWindow(std::move(libraryPath), result.Window);
        return result;
    }

    bool DestroyWindow(std::shared_ptr<Window>& window)
    {
        if (window == nullptr)
        {
            NEB_CORE_LOG_WARN("Attempted to destroy a null window");
            return false;
        }

        auto [success, module] = Loader::RemoveWindow(window);
        if (success)
        {
            NEB_ASSERT(window.use_count() == 1, "Window is still being referenced by another object");
            window.reset();
            Loader::ModuleRemoveIfEmpty(module);
            return true;
        }

        NEB_CORE_LOG_WARN("Attempted to destroy a window that was not created by Nebula");
        return false;
    }
} // namespace Nebula
