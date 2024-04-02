#include "Window.hpp"

// TODO: Make cross-platform
#include <NebulaCore/Log.hpp>
#include <NebulaCore/Util.hpp>
#include <NebulaCore/Assert.hpp>

#ifdef NEBULA_PLATFORM_WINDOWS
    #include <Windows.h>
#else
    #error "Unsupported platform"
#endif


namespace Nebula
{
#ifdef NEBULA_PLATFORM_WINDOWS
    using ModuleHandle = HINSTANCE__*;
#endif

    //NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static std::vector<std::pair<std::string, ModuleHandle>> s_LoadedLibraries = {};
    //NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static std::vector<std::pair<std::string, std::vector<std::shared_ptr<Window>>>> s_OpenWindows = {};

    static ModuleHandle LoadGraphicsLibrary(const std::string& libraryPath)
    {
        auto iterator = std::find_if(s_LoadedLibraries.begin(), s_LoadedLibraries.end(),
                                     [&libraryPath](const auto& pair) { return pair.first == libraryPath; });
        if (iterator != s_LoadedLibraries.end())
        {
            return iterator->second;
        }

        NEB_CORE_LOG_TRACE("[Renderer] Loading library: {0}", libraryPath);
        ModuleHandle handle = LoadLibrary(libraryPath.c_str());
        if (handle == nullptr)
        {
            return nullptr;
        }

        s_LoadedLibraries.emplace_back(libraryPath, handle);
        NEB_CORE_LOG_TRACE("[Renderer] Loaded library");
        return handle;
    }

    static void InsertWindow(const std::string& libraryPath, std::shared_ptr<Window> window)
    {
        auto iterator = std::find_if(s_OpenWindows.begin(), s_OpenWindows.end(),
                                     [&libraryPath](const auto& pair) { return pair.first == libraryPath; });
        if (iterator != s_OpenWindows.end())
        {
            iterator->second.push_back(window);
        }
        else
        {
            s_OpenWindows.emplace_back(libraryPath, std::vector<std::shared_ptr<Window>> {window});
        }
    }

    static void FreeGraphicsLibrary(ModuleHandle handle)
    {
        if (handle == nullptr)
        {
            return;
        }

        auto iterator = std::find_if(s_LoadedLibraries.begin(), s_LoadedLibraries.end(),
                                     [handle](const auto& pair) { return pair.second == handle; });

        if (iterator != s_LoadedLibraries.end())
        {
            NEB_CORE_LOG_TRACE("[Renderer] Unloading library: {0}", iterator->first);
            s_LoadedLibraries.erase(iterator);
            FreeLibrary(handle);
            NEB_CORE_LOG_TRACE("[Renderer] Unloaded library");
        }
    }

    WindowCreationResult CreateGLFWWindow(std::string libraryPath, WindowProps<OpenGLContext>& props)
    {
        libraryPath += NEBULA_PLATFORM_SHARED_LIBRARY_EXTENSION;
        ModuleHandle library = LoadGraphicsLibrary(libraryPath);
        if (library == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to load library: {0}", libraryPath);
            return {nullptr, WindowCreationResult::ErrorType::LibraryLoadFailed};
        }

        using CreateGLFWWindowFunc = WindowCreationResult (*)(WindowProps<OpenGLContext>&);
        auto* createGLFWWindow =
            reinterpret_cast<CreateGLFWWindowFunc>(GetProcAddress(library, "nebulaGraphicsOpenGLCreateGLFWWindow"));

        if (createGLFWWindow == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to load symbol: InternalCreateGLFWWindow");
            return {nullptr, WindowCreationResult::ErrorType::SymbolLoadFailed};
        }

        auto result = createGLFWWindow(props);
        if (result.Window == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to create window");
            return {nullptr, WindowCreationResult::ErrorType::WindowCreationFailed};
        }
        InsertWindow(libraryPath, result.Window);
        return result;
    }

    bool DestroyWindow(std::shared_ptr<Window>& window)
    {
        if (window == nullptr)
        {
            NEB_CORE_LOG_WARN("Attempted to destroy a null window");
            return false;
        }

        for (auto& [libraryPath, windows] : s_OpenWindows)
        {
            auto iterator = std::find(windows.begin(), windows.end(), window);
            if (iterator != windows.end())
            {
                windows.erase(iterator);
                iterator = windows.begin(); // Reset the iterator, since it is invalidated after erasing
                if (windows.empty())
                {
                    NEB_ASSERT(window.use_count() == 1, "Window is still being referenced by another shared pointer");
                    window = nullptr; // Reset the shared pointer, so that the window is destroyed
                    FreeGraphicsLibrary(LoadGraphicsLibrary(libraryPath));
                }
                else
                {
                    NEB_CORE_LOG_TRACE("Window was destroyed, but library is still loaded ({0} windows remaining)",
                                       windows.size());
                }
                return true;
            }
        }

        NEB_CORE_LOG_WARN("Attempted to destroy a window that was not created by Nebula");
        return false;
    }
} // namespace Nebula
