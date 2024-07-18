#pragma once

#include "NebulaEngine/Core/Result.hpp"
#include "NebulaEngine/Core/Window.hpp"
#include "NebulaEngine/Renderer/GraphicsContext.hpp"

#include <GLFW/glfw3.h>

namespace Nebula
{
    struct MacOSWindowData : public BaseWindowData
    {
        explicit MacOSWindowData(std::string title, std::uint32_t width, std::uint32_t height) :
            BaseWindowData(std::move(title), width, height)
        {
        }
    };

    class MacOSWindow : public Window
    {
    public:
        explicit MacOSWindow(GLFWwindow* window, MacOSWindowData data);
        ~MacOSWindow() override;
        MacOSWindow(const MacOSWindow&)            = delete;
        MacOSWindow& operator=(const MacOSWindow&) = delete;
        MacOSWindow(MacOSWindow&&)                 = delete;
        MacOSWindow& operator=(MacOSWindow&&)      = delete;

        void BeginFrame() override;
        void EndFrame() override;
        void OnUpdate() override;
        void WaitForEvents() override;

        void SetEventCallback(const WindowEventCallback& callback) override { m_Data.EventCallback = callback; }

        RefPtr<GraphicsContext> GetGraphicsContext() const override { return m_Context; }
        Bounds GetViewportBounds() const override;
        std::uint32_t GetWidth() const override { return m_Data.Width; }
        std::uint32_t GetHeight() const override { return m_Data.Height; }
        void* GetNativeWindow() const override { return m_Window.Get(); }

        void SetVSync(bool enabled) override { m_Context->SetVSync(enabled); }
        bool IsVSync() const override { return m_Context->IsVSync(); }

        static Result<RefPtr<MacOSWindow>, std::string> Create(const WindowProps& props);
    private:
        OwnedPtr<GLFWwindow*> m_Window;
        MacOSWindowData m_Data;
        RefPtr<GraphicsContext> m_Context;
    };
} // namespace Nebula

