#pragma once

#include "NebulaEngine/Core/Window.hpp"
#include "NebulaEngine/Renderer/RenderContext.hpp"

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

        void OnUpdate() override;

        void SetEventCallback(const WindowEventCallback& callback) override { m_Data.EventCallback = callback; }

        void* GetNativeWindow() const override { return m_Window; }

        std::uint32_t GetWidth() const override { return m_Data.Width; }

        std::uint32_t GetHeight() const override { return m_Data.Height; }
    private:
        GLFWwindow* m_Window;
        MacOSWindowData m_Data;
        Ptr<RenderContext> m_RenderContext;
    };
} // namespace Nebula

