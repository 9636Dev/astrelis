#include "WindowHelper.hpp"
#include "GLFW/glfw3.h"

#include "Event/KeyEvent.hpp"
#include "Event/MouseEvent.hpp"
#include "Event/WindowEvent.hpp"
#include "NebulaCore/Log.hpp"
#include "NebulaCore/Util/Assert.hpp"

namespace Nebula
{
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static std::size_t s_WindowCount = 0;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static bool s_GLFWInitialized = false;

    Result<GLFWwindow*, WindowCreationError> WindowHelper::CreateWindow(const WindowProps& props)
    {
        if (!s_GLFWInitialized)
        {
            if (glfwInit() != GLFW_TRUE)
            {
                glfwTerminate();
                return WindowCreationError::GLFWInitFailed;
            }
            s_GLFWInitialized = true;
            NEBULA_CORE_LOG_DEBUG("Windowing: GLFW initialized");
        }

        GLFWwindow* window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
        if (window == nullptr)
        {
            return WindowCreationError::WindowCreationFailed;
        }

        s_WindowCount++;
        return window;
    }

    void WindowHelper::TerminateWindow(GLFWwindow* window)
    {
        glfwSetWindowUserPointer(window, nullptr);
        glfwDestroyWindow(window);

        if (--s_WindowCount == 0)
        {
            glfwTerminate();
            s_GLFWInitialized = false;
            NEBULA_CORE_LOG_DEBUG("Windowing: GLFW terminated");
        }
    }

    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
    void WindowHelper::SetEventCallbacks(GLFWwindow* window, WindowData& data)
    {
        glfwSetWindowUserPointer(window, &data);

        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            WindowResizedEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            WindowClosedEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action,
                                      [[maybe_unused]] int mods) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            // NOLINTNEXTLINE(readability-simplify-boolean-expr)
            NEBULA_CORE_ASSERT(action == GLFW_PRESS || action == GLFW_RELEASE || action == GLFW_REPEAT,
                               "Invalid action type");
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

            switch (action)
            {
            case GLFW_PRESS: {
                KeyPressedEvent event(static_cast<KeyCode>(key), false);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(static_cast<KeyCode>(key));
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(static_cast<KeyCode>(key), true);
                data.EventCallback(event);
                break;
            }
            default:
                break;
            }
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            // NOLINTNEXTLINE(readability-simplify-boolean-expr)
            NEBULA_CORE_ASSERT(action == GLFW_PRESS || action == GLFW_RELEASE, "Invalid action type");
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

            switch (action)
            {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(static_cast<MouseCode>(button));
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
                data.EventCallback(event);
                break;
            }
            default:
                break;
            }
        });

        glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            MouseScrolledEvent event(static_cast<float>(xoffset), static_cast<float>(yoffset));
            data.EventCallback(event);
        });

        glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

            if (focused != 0)
            {
                WindowFocusedEvent event;
                data.EventCallback(event);
            }
            else
            {
                WindowLostFocusEvent event;
                data.EventCallback(event);
            }
        });

        glfwSetWindowIconifyCallback(window, [](GLFWwindow* window, int iconified) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

            if (iconified != 0)
            {
                WindowMinimizedEvent event;
                data.EventCallback(event);
            }
            else
            {
                WindowRestoredEvent event;
                data.EventCallback(event);
            }
        });

        glfwSetWindowMaximizeCallback(window, [](GLFWwindow* window, int maximized) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

            if (maximized != 0)
            {
                WindowMaximizedEvent event;
                data.EventCallback(event);
            }
            else
            {
                WindowRestoredEvent event;
                data.EventCallback(event);
            }
        });

        glfwSetWindowRefreshCallback(window, [](GLFWwindow* window) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            WindowRefreshEvent event;
            data.EventCallback(event);
        });

        glfwSetWindowPosCallback(window, [](GLFWwindow* window, int xpos, int ypos) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            WindowMovedEvent event(xpos, ypos);
            data.EventCallback(event);
        });

        glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            WindowResizedEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowContentScaleCallback(window, [](GLFWwindow* window, float xscale, float yscale) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            WindowScaleEvent event(xscale, yscale);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            WindowClosedEvent event;
            data.EventCallback(event);
        });

        glfwSetCursorEnterCallback(window, [](GLFWwindow* window, int entered) {
            auto& data = WindowHelper::GetUserData<WindowData>(window);
            NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

            if (entered != 0)
            {
                MouseEnteredEvent event;
                data.EventCallback(event);
            }
            else
            {
                MouseLeftEvent event;
                data.EventCallback(event);
            }
        });
    }
} // namespace Nebula
