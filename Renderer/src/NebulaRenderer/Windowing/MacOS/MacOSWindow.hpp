#pragma once

#include "../Window.hpp"
#include <GLFW/glfw3.h>

/**
* @file MacOSWindow.hpp
* @brief MacOS implementation of the Window class, for now it is using OpenGL
*/

namespace Nebula
{
    struct MacOSWindowData : public WindowData
    {
    };

    class MacOSWindow : public Window
    {
    public:
        explicit MacOSWindow(GLFWwindow* window);
        MacOSWindow(const MacOSWindow&)            = delete;
        MacOSWindow& operator=(const MacOSWindow&) = delete;
        MacOSWindow(MacOSWindow&&)                 = delete;
        MacOSWindow& operator=(MacOSWindow&&)      = delete;
        ~MacOSWindow() override;

        [[nodiscard]] bool ShouldClose() const noexcept override;
        void PollEvents() noexcept override;
        void SwapBuffers() noexcept override;

        inline bool IsOk() const noexcept override { return m_Window != nullptr; }

        inline void SetEventCallback(const EventCallbackFn& callback) noexcept override
        {
            m_Data.EventCallback = callback;
        }

        inline void* GetNativeWindow() const noexcept override { return m_Window; }
    private:
        GLFWwindow* m_Window;
        MacOSWindowData m_Data;
    };
} // namespace Nebula
