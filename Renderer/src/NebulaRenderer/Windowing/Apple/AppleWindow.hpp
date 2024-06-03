#pragma once

#include "../Window.hpp"
#include <GLFW/glfw3.h>

/**
* @file AppleWindow.hpp
* @brief Apple implementation of the Window class, for now it is using OpenGL
*/

namespace Nebula
{
    struct AppleWindowData : public WindowData
    {
    };

    class AppleWindow : public Window
    {
    public:
        explicit AppleWindow(GLFWwindow* window);
        AppleWindow(const AppleWindow&)            = delete;
        AppleWindow& operator=(const AppleWindow&) = delete;
        AppleWindow(AppleWindow&&)                 = delete;
        AppleWindow& operator=(AppleWindow&&)      = delete;
        ~AppleWindow() override;

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
        AppleWindowData m_Data;
    };
} // namespace Nebula
