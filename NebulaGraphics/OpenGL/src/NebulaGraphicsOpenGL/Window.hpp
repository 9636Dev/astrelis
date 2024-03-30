#pragma once

#include "Core.hpp"
#include <NebulaGraphicsCore/Window.hpp>
#include <GLFW/glfw3.h>

namespace Nebula
{
    class GLFWWindow : public Window
    { 
    public:
        GLFWWindow();
        ~GLFWWindow() override;
        GLFWWindow(const GLFWWindow&) = delete;
        GLFWWindow& operator=(const GLFWWindow&) = delete;
        GLFWWindow(GLFWWindow&&) = delete;
        GLFWWindow& operator=(GLFWWindow&&) = delete;
        
        void SwapBuffers() override;
        void PollEvents() override;
        [[nodiscard]] bool ShouldClose() const override;

    private:
        GLFWwindow* m_Window;
    };
} // namespace Nebula

