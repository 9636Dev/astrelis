#pragma once

#include <GLFW/glfw3.h>
#include <NebulaGraphicsCore/Window.hpp>

namespace Nebula
{
    struct GLFWWindowData
    {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;

        Window::EventCallbackFunction EventCallback;

        GLFWWindowData(std::string title, unsigned int width, unsigned int height, bool vsync) :
            Title(std::move(title)),
            Width(width),
            Height(height),
            VSync(vsync)
        {
        }
    };

    class GLFWWindow : public Window
    {
    public:
        // We don't construct in a constructor, because it restricts error handling
        explicit GLFWWindow(GLFWwindow* window, GLFWWindowData data);
        ~GLFWWindow() override;
        GLFWWindow(const GLFWWindow&)            = delete;
        GLFWWindow& operator=(const GLFWWindow&) = delete;
        GLFWWindow(GLFWWindow&&)                 = delete;
        GLFWWindow& operator=(GLFWWindow&&)      = delete;

        void SwapBuffers() override;
        void PollEvents() override;
        [[nodiscard]] bool ShouldClose() const override;

        void SetEventCallback(const EventCallbackFunction& callback) override { m_Data.EventCallback = callback; }

        void SetVSync(bool enabled) override;

        [[nodiscard]] bool IsVSync() const override { return m_Data.VSync; }

        [[nodiscard]] unsigned int GetWidth() const override { return m_Data.Width; }

        [[nodiscard]] unsigned int GetHeight() const override { return m_Data.Height; }

        // GLFW Specific
        void MakeContextCurrent() const { glfwMakeContextCurrent(m_Window); }

        [[nodiscard]] GLFWwindow* GetNativeWindow() const { return m_Window; }
    private:
        GLFWwindow* m_Window;
        GLFWWindowData m_Data;
    };
} // namespace Nebula

