#pragma once

#include "NebulaEngine/Core/Pointer.hpp"
#include "NebulaEngine/Core/Result.hpp"
#include "NebulaEngine/Core/Window.hpp"
#include "NebulaEngine/Renderer/RenderContext.hpp"

#include <GLFW/glfw3.h>
#include <string>

namespace Nebula
{
    struct LinuxWindowData : public BaseWindowData
    {
        explicit LinuxWindowData(std::string title, std::uint32_t width, std::uint32_t height) :
            BaseWindowData(std::move(title), width, height)
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

        void SetEventCallback(const WindowEventCallback& callback) override { m_Data.EventCallback = callback; }

        void* GetNativeWindow() const override { return m_Window; }

        std::uint32_t GetWidth() const override { return m_Data.Width; }

        std::uint32_t GetHeight() const override { return m_Data.Height; }

        static Result<Ptr<LinuxWindow>, std::string> Create(const WindowProps& props);

        Bounds GetViewportBounds() const override;
    private:
        GLFWwindow* m_Window;
        LinuxWindowData m_Data;
        Ptr<RenderContext> m_RenderContext;
    };
} // namespace Nebula
