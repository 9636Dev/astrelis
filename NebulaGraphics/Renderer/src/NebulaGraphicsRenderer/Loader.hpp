#pragma once

#include <NebulaGraphicsCore/Renderer.hpp>
#include <NebulaGraphicsCore/Window.hpp>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace Nebula
{
    class Loader
    {
    public:
        struct ModuleHandle
        {
            void* Handle;

            explicit ModuleHandle(void* handle) : Handle(handle) {}
        };

        static ModuleHandle LoadGraphicsLibrary(const std::string& libraryPath);
        static void InsertWindow(std::string libraryPath, std::shared_ptr<Window> window);
        static void InsertRenderer(std::string libraryPath, std::shared_ptr<Renderer> renderer);
        static void FreeGraphicsLibrary(ModuleHandle handle);
        static void* LoadSymbol(ModuleHandle handle, const std::string& symbolName);
        // Returns: <Success, ModulePath>
        static std::pair<bool, std::string> RemoveWindow(const std::shared_ptr<Window>& window);
        static std::pair<bool, std::string> RemoveRenderer(const std::shared_ptr<Renderer>& renderer);
        static bool ModuleRemoveIfEmpty(const std::string& libraryPath);
    private:
        //NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        inline static std::vector<std::pair<std::string, ModuleHandle>> s_LoadedLibraries = {};
        //NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        inline static std::vector<std::pair<std::string, std::vector<std::shared_ptr<Window>>>> s_Windows = {};
        //NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
        inline static std::vector<std::pair<std::string, std::vector<std::shared_ptr<Renderer>>>> s_Renderers = {};
    };
} // namespace Nebula

