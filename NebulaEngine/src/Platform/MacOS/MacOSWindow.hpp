#pragma once

#include "NebulaEngine/Core/Result.hpp"
#include "NebulaEngine/Core/Window.hpp"
#include "NebulaEngine/Renderer/GraphicsContext.hpp"

#include <GLFW/glfw3.h>
#include <MacTypes.h>

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

        RefPtr<GraphicsContext> GetGraphicsContext() const override { return m_Context; }
        Bounds GetViewportBounds() const override;

        static Result<RefPtr<MacOSWindow>, std::string> Create(const WindowProps& props);
    private:
        OwnedPtr<GLFWwindow*> m_Window;
        MacOSWindowData m_Data;
        RefPtr<GraphicsContext> m_Context;
    };
} // namespace Nebula

