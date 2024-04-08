#pragma once

#include <memory>

namespace Nebula
{
    class Renderer
    {
    public:
        Renderer() = default;
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        virtual ~Renderer() = default;

        virtual void Render() = 0;
    };

    struct RendererCreationResult
    {
        enum class ErrorType
        {
            None,
            LibraryLoadFailed,
            SymbolLoadFailed,
            InvalidArguments,
            ContextNotSupported,
            RendererInitFailed,
        };

        std::shared_ptr<Renderer> Renderer;
        ErrorType Error;
    };
} // namespace Nebula
