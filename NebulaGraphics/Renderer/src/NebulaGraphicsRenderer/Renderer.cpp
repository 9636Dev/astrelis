#pragma once

#include "Renderer.hpp"
#include "Loader.hpp"

#include "NebulaCore/Assert.hpp"
#include "NebulaCore/Log.hpp"

namespace Nebula
{
    RendererCreationResult
        CreateGLRenderer(std::string libraryPath, const std::shared_ptr<Window>& window, const OpenGLContext& context)
    {
        Loader::ModuleHandle library = Loader::LoadGraphicsLibrary(libraryPath);
        if (library.Handle == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to load library: {0}", libraryPath);
            return {nullptr, RendererCreationResult::ErrorType::LibraryLoadFailed};
        }

        using CreateGLFWRendererFunc = RendererCreationResult (*)(const OpenGLContext&, const std::shared_ptr<Window>&);
        auto* createGLFWRenderer =
            /* NOLINT */ reinterpret_cast<CreateGLFWRendererFunc>(
                Loader::LoadSymbol(library, "nebulaGraphicsOpenGLCreateOpenGLRenderer"));

        if (createGLFWRenderer == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to load symbol: nebulaGraphicsOpenGLCreateOpenGLRenderer");
            return {nullptr, RendererCreationResult::ErrorType::SymbolLoadFailed};
        }

        auto result = createGLFWRenderer(context, window);
        if (result.Renderer == nullptr)
        {
            NEB_CORE_LOG_ERROR("Failed to create Renderer");
            return result;
        }
        Loader::InsertRenderer(std::move(libraryPath), result.Renderer);
        return result;
    }

    bool DestroyRenderer(std::shared_ptr<Renderer>& window)
    {
        if (window == nullptr)
        {
            NEB_CORE_LOG_WARN("Attempted to destroy a null renderer");
            return false;
        }

        auto [success, module] = Loader::RemoveRenderer(window);
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
