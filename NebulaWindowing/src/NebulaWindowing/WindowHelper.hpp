#pragma once

#include <GLFW/glfw3.h>

#include <cstdint>
#include <functional>
#include <string>

#include "Event/Event.hpp"
#include "Event/KeyEvent.hpp"
#include "Event/MouseEvent.hpp"
#include "Event/WindowEvent.hpp"
#include "NebulaCore/Util/Assert.hpp"
#include "NebulaCore/Util/Result.hpp"

namespace Nebula
{
    struct WindowProps
    {
        std::string Title;
        std::int32_t Width;
        std::int32_t Height;
    };

    enum class WindowCreationError
    {
        GLFWInitFailed,
        WindowCreationFailed
    };

    // A ceoncept to check a type have a 'EventCallback' variable of type 'std::function<void(Event&)>'
    template<typename T>
    concept HasEventCallback = requires(T type) {
        { type.EventCallback } -> std::convertible_to<std::function<void(Event&)>>;
    };

    class WindowHelper
    {
    public:
        static Result<GLFWwindow*, WindowCreationError> CreateWindow(const WindowProps& props);
        static void TerminateWindow(GLFWwindow* window);

        template<typename T> static T& GetUserData(GLFWwindow* window)
        {
            return *static_cast<T*>(glfwGetWindowUserPointer(window));
        }

        // NOLINTNEXTLINE(readability-function-cognitive-complexity)
        template<HasEventCallback T> static void SetEventCallback(GLFWwindow* window, T& data)
        {
            glfwSetWindowUserPointer(window, &data);

            glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
                auto& data = WindowHelper::GetUserData<T>(window);
                NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
                WindowResizedEvent event(width, height);
                data.EventCallback(event);
            });

            glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
                auto& data = WindowHelper::GetUserData<T>(window);
                NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
                WindowClosedEvent event;
                data.EventCallback(event);
            });

            glfwSetKeyCallback(window, [](GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action,
                                          [[maybe_unused]] int mods) {
                auto& data = WindowHelper::GetUserData<T>(window);
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
                auto& data = WindowHelper::GetUserData<T>(window);
                NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
                MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
                data.EventCallback(event);
            });

            glfwSetMouseButtonCallback(window,
                                       [](GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
                auto& data = WindowHelper::GetUserData<T>(window);
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
                auto& data = WindowHelper::GetUserData<T>(window);
                NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
                MouseScrolledEvent event(static_cast<float>(xoffset), static_cast<float>(yoffset));
                data.EventCallback(event);
            });

            glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused) {
                auto& data = WindowHelper::GetUserData<T>(window);
                NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

                if (focused)
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
                auto& data = WindowHelper::GetUserData<T>(window);
                NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

                if (iconified)
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
                auto& data = WindowHelper::GetUserData<T>(window);
                NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

                if (maximized)
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
                auto& data = WindowHelper::GetUserData<T>(window);
                NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
                WindowRefreshEvent event;
                data.EventCallback(event);
            });

            glfwSetWindowPosCallback(window, [](GLFWwindow* window, int xpos, int ypos) {
                auto& data = WindowHelper::GetUserData<T>(window);
                NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
                WindowMovedEvent event(xpos, ypos);
                data.EventCallback(event);
            });

            glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
                auto& data = WindowHelper::GetUserData<T>(window);
                NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
                WindowResizedEvent event(width, height);
                data.EventCallback(event);
            });

            glfwSetWindowContentScaleCallback(window, [](GLFWwindow* window, float xscale, float yscale) {
                auto& data = WindowHelper::GetUserData<T>(window);
                NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
                WindowScaleEvent event(xscale, yscale);
                data.EventCallback(event);
            });

            glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
                auto& data = WindowHelper::GetUserData<T>(window);
                NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
                WindowClosedEvent event;
                data.EventCallback(event);
            });

            glfwSetCursorEnterCallback(window, [](GLFWwindow* window, int entered) {
                auto& data = WindowHelper::GetUserData<T>(window);
                NEBULA_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

                if (entered)
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
    };
}; // namespace Nebula
