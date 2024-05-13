#include "NebulaGraphicsCore/Basic/Renderer.hpp"
#include "Renderer.hpp"

#include "Loader.hpp"
#include "NebulaCore/Assert.hpp"
#include "NebulaCore/Log.hpp"

namespace Nebula
{
    Basic::RendererCreationResult
        CreateBasicGLRenderer(std::string libraryPath, const std::shared_ptr<Window>& window, const OpenGLContext& context)
    {
        Loader::ModuleHandle library = Loader::LoadGraphicsLibrary(libraryPath);
        if (library.Handle == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to load library: {0}", libraryPath);
            return {nullptr, RendererCreationResult::ErrorType::LibraryLoadFailed};
        }

        using CreateGLFWRendererFunc = Basic::RendererCreationResult (*)(const OpenGLContext&, const std::shared_ptr<Window>&);
        auto* createGLFWRenderer =
            /* NOLINT */ reinterpret_cast<CreateGLFWRendererFunc>(
                Loader::LoadSymbol(library, "nebulaGraphicsOpenGLCreateBasicOpenGLRenderer"));

        if (createGLFWRenderer == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to load symbol: nebulaGraphicsOpenGLCreateBasicOpenGLRenderer");
            return {nullptr, RendererCreationResult::ErrorType::SymbolLoadFailed};
        }

        auto result = createGLFWRenderer(context, window);
        if (result.Renderer == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to create Renderer");
            return result;
        }

        Loader::InsertBasicRenderer(std::move(libraryPath), result.Renderer);
        return result;
    }

    Basic::RendererCreationResult
        CreateBaicMetalRenderer(std::string libraryPath, const std::shared_ptr<Window>& window, const MetalContext& context)
    {
        Loader::ModuleHandle library = Loader::LoadGraphicsLibrary(libraryPath);
        if (library.Handle == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to load library: {0}", libraryPath);
            return {nullptr, RendererCreationResult::ErrorType::LibraryLoadFailed};
        }

        using CreateGLFWRendererFunc = Basic::RendererCreationResult (*)(const MetalContext&, const std::shared_ptr<Window>&);
        auto* createMetalRenderer =
            /* NOLINT */ reinterpret_cast<CreateGLFWRendererFunc>(
                Loader::LoadSymbol(library, "nebulaGraphicsMetalCreateMetalRenderer"));

        if (createMetalRenderer == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to load symbol: nebulaGraphicsMetalCreateMetalRenderer");
            return {nullptr, RendererCreationResult::ErrorType::SymbolLoadFailed};
        }

        auto result = createMetalRenderer(context, window);
        if (result.Renderer == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to create Renderer");
            return result;
        }
        Loader::InsertBasicRenderer(std::move(libraryPath), result.Renderer);
        return result;
    }

    bool DestroyBasicRenderer(std::shared_ptr<Basic::Renderer>& window)
    {
        if (window == nullptr)
        {
            NEB_CORE_LOG_WARN("Attempted to destroy a null renderer");
            return false;
        }

        auto [success, module] = Loader::RemoveBasicRenderer(window);
        if (success)
        {
            NEB_ASSERT(window.use_count() == 1, "Renderer is still being referenced by another object");
            window.reset();
            Loader::ModuleRemoveIfEmpty(module);
            return true;
        }

        NEB_CORE_LOG_WARN("Attempted to destroy a renderer that was not created by Nebula");
        return false;
    }
} // namespace Nebula
