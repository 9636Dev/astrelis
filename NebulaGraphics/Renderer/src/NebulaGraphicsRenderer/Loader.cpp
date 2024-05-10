#include "Loader.hpp"

#include "NebulaCore/Core.hpp"
#include "NebulaCore/Log.hpp"

#ifdef NEBULA_PLATFORM_WINDOWS
    #include <Windows.h>
#elif defined(NEBULA_PLATFORM_APPLE) || defined(NEBULA_PLATFORM_LINUX)
    #include <dlfcn.h>
#else
    #error "Platform not supported"
#endif

namespace Nebula
{
    Loader::ModuleHandle Loader::LoadGraphicsLibrary(const std::string& libraryPath)
    {
        auto iterator = std::find_if(s_LoadedLibraries.begin(), s_LoadedLibraries.end(),
                                     [&libraryPath](const auto& pair) { return pair.first == libraryPath; });
        if (iterator != s_LoadedLibraries.end())
        {
            return iterator->second;
        }

        NEB_CORE_LOG_TRACE("[Renderer] Loading library: {0}", libraryPath);
#ifdef NEBULA_PLATFORM_WINDOWS
        auto handle = ModuleHandle(LoadLibrary(libraryPath.c_str()));
#else
        auto handle = ModuleHandle(dlopen(libraryPath.c_str(), RTLD_NOW | RTLD_GLOBAL));
#endif

        if (handle.Handle == nullptr)
        {
            return handle;
        }

        s_LoadedLibraries.emplace_back(libraryPath, handle);
        NEB_CORE_LOG_TRACE("[Renderer] Loaded library");
        return handle;
    }

    template<typename T>
    static inline void InsertObject(std::string libraryPath,
                                    std::shared_ptr<T> object,
                                    std::vector<std::pair<std::string, std::vector<std::shared_ptr<T>>>>& objects)
    {
        auto iterator = std::find_if(objects.begin(), objects.end(),
                                     [&libraryPath](const auto& pair) { return pair.first == libraryPath; });
        if (iterator != objects.end())
        {
            iterator->second.push_back(std::move(object));
        }
        else
        {
            objects.emplace_back(std::move(libraryPath), std::vector<std::shared_ptr<T>> {std::move(object)});
        }
    }

    void Loader::InsertWindow(std::string libraryPath, std::shared_ptr<Window> window)
    {
        InsertObject(std::move(libraryPath), std::move(window), s_Windows);
    }

    void Loader::InsertRenderer(std::string libraryPath, std::shared_ptr<Renderer> renderer)
    {
        InsertObject(std::move(libraryPath), std::move(renderer), s_Renderers);
    }

    template<typename T>
    static inline std::pair<bool, std::string>
        RemoveObject(const std::shared_ptr<T>& window,
                     std::vector<std::pair<std::string, std::vector<std::shared_ptr<T>>>>& objects)
    {
        for (auto& pair : objects)
        {
            auto& windows = pair.second;
            auto iterator = std::find(windows.begin(), windows.end(), window);
            if (iterator != windows.end())
            {
                windows.erase(iterator);
                return std::make_pair(true, pair.first);
            }
        }
        return std::make_pair(false, std::string());
    }

    std::pair<bool, std::string> Loader::RemoveWindow(const std::shared_ptr<Window>& window)
    {
        return RemoveObject(window, s_Windows);
    }

    std::pair<bool, std::string> Loader::RemoveRenderer(const std::shared_ptr<Renderer>& renderer)
    {
        return RemoveObject(renderer, s_Renderers);
    }

    bool Loader::ModuleRemoveIfEmpty(const std::string& libraryPath)
    {
        auto windowIterator = std::find_if(s_Windows.begin(), s_Windows.end(),
                                           [&libraryPath](const auto& pair) { return pair.first == libraryPath; });

        if (windowIterator != s_Windows.end())
        {
            if (!windowIterator->second.empty())
            {
                return false;
            }
            s_Windows.erase(windowIterator);
        }


        auto rendererIterator = std::find_if(s_Renderers.begin(), s_Renderers.end(),
                                             [&libraryPath](const auto& pair) { return pair.first == libraryPath; });

        if (rendererIterator != s_Renderers.end())
        {
            if (!rendererIterator->second.empty())
            {
                return false;
            }
            s_Renderers.erase(rendererIterator);
        }


        // We can now safely unload the library
        FreeGraphicsLibrary(LoadGraphicsLibrary(libraryPath));
        return true;
    }

    void Loader::FreeGraphicsLibrary(ModuleHandle handle)
    {
        if (handle.Handle == nullptr)
        {
            return;
        }

        auto iterator = std::find_if(s_LoadedLibraries.begin(), s_LoadedLibraries.end(),
                                     [handle](const auto& pair) { return pair.second.Handle == handle.Handle; });

        if (iterator != s_LoadedLibraries.end())
        {
            NEB_CORE_LOG_TRACE("[Renderer] Unloading library: {0}", iterator->first);
            s_LoadedLibraries.erase(iterator);
#ifdef NEBULA_PLATFORM_WINDOWS
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
            FreeLibrary(reinterpret_cast<HMODULE>(handle.Handle));
#else
            dlclose(handle.Handle);
#endif
            NEB_CORE_LOG_TRACE("[Renderer] Unloaded library");
        }
    }

    void* Loader::LoadSymbol(ModuleHandle handle, const std::string& symbolName)
    {
        if (handle.Handle == nullptr)
        {
            return nullptr;
        }

#ifdef NEBULA_PLATFORM_WINDOWS
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        return reinterpret_cast<void*>(GetProcAddress(reinterpret_cast<HMODULE>(handle.Handle), symbolName.c_str()));
#else
        return dlsym(handle.Handle, symbolName.c_str());
#endif
    }
} // namespace Nebula

