#pragma once

#include <GLFW/glfw3.h>

#include "Astrelis/Core/Result.hpp"
#include "Astrelis/Core/Window.hpp"
#include "Astrelis/Renderer/GraphicsContext.hpp"

namespace Astrelis
{
    struct MacOSWindowData : public BaseWindowData
    {
        explicit MacOSWindowData(std::string title, Dimension2Du dimensions) :
            BaseWindowData(std::move(title), dimensions)
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
        Rect2Di GetViewportBounds() const override;
        std::uint32_t GetWidth() const override { return m_Data.Dimensions.Width; }
        std::uint32_t GetHeight() const override { return m_Data.Dimensions.Height; }
        void* GetNativeWindow() const override { return m_Window.Get(); }

        void SetVSync(bool enabled) override { m_Context->SetVSync(enabled); }
        bool IsVSync() const override { return m_Context->IsVSync(); }

        static Result<RefPtr<MacOSWindow>, std::string> Create(const WindowProps& props);
    private:
        OwnedPtr<GLFWwindow*> m_Window;
        MacOSWindowData m_Data;
        RefPtr<GraphicsContext> m_Context;
    };
} // namespace Astrelis

