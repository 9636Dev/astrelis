#pragma once

#include "NebulaEngine/Core/Result.hpp"
#include "NebulaEngine/Core/Window.hpp"
#include "NebulaEngine/Renderer/GraphicsContext.hpp"

#include <GLFW/glfw3.h>

namespace Nebula
{
    struct WindowsWindowData : public BaseWindowData
    {
        explicit WindowsWindowData(std::string title, std::uint32_t width, std::uint32_t height) :
            BaseWindowData(std::move(title), width, height)
        {
        }
    };

    class WindowsWindow : public Window
    {
    public:
        explicit WindowsWindow(GLFWwindow* window, WindowsWindowData data);
        ~WindowsWindow() override;
        WindowsWindow(const WindowsWindow&)            = delete;
        WindowsWindow& operator=(const WindowsWindow&) = delete;
        WindowsWindow(WindowsWindow&&)                 = delete;
        WindowsWindow& operator=(WindowsWindow&&)      = delete;

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

        static Result<RefPtr<WindowsWindow>, std::string> Create(const WindowProps& props);
    private:
        OwnedPtr<GLFWwindow*> m_Window;
        WindowsWindowData m_Data;
        RefPtr<GraphicsContext> m_Context;
    };
} // namespace Nebula


