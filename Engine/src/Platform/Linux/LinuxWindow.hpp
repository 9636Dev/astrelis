#pragma once

#include <GLFW/glfw3.h>

#include "Astrelis/Core/Result.hpp"
#include "Astrelis/Core/Window.hpp"
#include "Astrelis/Renderer/GraphicsContext.hpp"

namespace Astrelis
{
    struct LinuxWindowData : public BaseWindowData
    {
        explicit LinuxWindowData(std::string title, Dimension2Du dimensions) :
            BaseWindowData(std::move(title), dimensions)
        {
        }
    };

    class LinuxWindow : public Window
    {
    public:
        explicit LinuxWindow(GLFWwindow* window, LinuxWindowData data);
        ~LinuxWindow() override;
        LinuxWindow(const LinuxWindow&)            = delete;
        LinuxWindow& operator=(const LinuxWindow&) = delete;
        LinuxWindow(LinuxWindow&&)                 = delete;
        LinuxWindow& operator=(LinuxWindow&&)      = delete;

        void OnUpdate() override;
        void WaitForEvents() override;
        void BeginFrame() override;
        void EndFrame() override;

        void SetEventCallback(const WindowEventCallback& callback) override { m_Data.EventCallback = callback; }

        RefPtr<GraphicsContext> GetGraphicsContext() const override { return m_Context; }
        Rect2Di GetViewportBounds() const override;
        std::uint32_t GetWidth() const override { return m_Data.Dimensions.Width; }
        std::uint32_t GetHeight() const override { return m_Data.Dimensions.Height; }
        void* GetNativeWindow() const override { return m_Window.Get(); }

        bool IsVSync() const override { return m_Context->IsVSync(); }
        void SetVSync(bool enabled) override { m_Context->SetVSync(enabled); }

        static Result<RefPtr<LinuxWindow>, std::string> Create(const WindowProps& props);
    private:
        OwnedPtr<GLFWwindow*> m_Window;
        LinuxWindowData m_Data;
        RefPtr<GraphicsContext> m_Context;
    };
} // namespace Astrelis


