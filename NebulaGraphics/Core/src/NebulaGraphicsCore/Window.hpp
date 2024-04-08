#pragma once

#include <functional>
#include <memory>
#include <string>

#include "Event/Event.hpp"

namespace Nebula
{
    class Window
    {
    public:
        using EventCallbackFunction = std::function<void(Event&)>;

        Window()                         = default;
        virtual ~Window()                = default;
        Window(const Window&)            = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&)                 = delete;
        Window& operator=(Window&&)      = delete;

        [[nodiscard]] virtual bool ShouldClose() const = 0;
        virtual void SwapBuffers()                     = 0;
        virtual void PollEvents()                      = 0;

        virtual void SetEventCallback(const EventCallbackFunction& callback) = 0;
        virtual void SetVSync(bool enabled)                                  = 0;
        [[nodiscard]] virtual bool IsVSync() const                           = 0;

        [[nodiscard]] virtual unsigned int GetWidth() const  = 0;
        [[nodiscard]] virtual unsigned int GetHeight() const = 0;
    };

    struct RenderFrameworkContext
    {
    };

    struct OpenGLContext : public RenderFrameworkContext
    {
        /* The Major version will be 0xFF if it fails */
        std::uint8_t MajorVersion;
        std::uint8_t MinorVersion;

        OpenGLContext(std::uint8_t majorVersion, std::uint8_t minorVersion) :
            MajorVersion(majorVersion),
            MinorVersion(minorVersion)
        {
        }
    };

    template<typename T>
        requires std::is_base_of_v<RenderFrameworkContext, T>
    struct WindowProps
    {
    public:
        std::string Title;
        unsigned int Width;
        unsigned int Height;
        T Context;

        explicit WindowProps(std::string title, std::uint32_t width, std::uint32_t height, T context) :
            Title(std::move(title)),
            Width(width),
            Height(height),
            Context(std::move(context))
        {
        }
    };

    struct WindowCreationResult
    {
        enum class ErrorType
        {
            None,
            LibraryLoadFailed,
            SymbolLoadFailed,
            WindowCreationFailed,
            ContextCreationFailed,
            RendererInitFailed,
            InvalidArguments
        };

        std::shared_ptr<Window> Window;
        ErrorType Error;
    };
} // namespace Nebula
