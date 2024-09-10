#include "GLFWWindowHelper.hpp"

#include "Astrelis/Core/Assert.hpp"
#include "Astrelis/Core/Result.hpp"
#include "Astrelis/Events/KeyEvent.hpp"
#include "Astrelis/Events/MouseEvent.hpp"
#include "Astrelis/Events/WindowEvent.hpp"

namespace Astrelis
{
    Result<GLFWwindow*, std::string> GLFWWindowHelper::CreateGLWindow(const WindowProps& props)
    {
        if (!InitGLFW())
        {
            return Result<GLFWwindow*, std::string>::Err("Failed to initialize GLFW");
        }

        // Set the window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create the window
        if (props.Dimensions.Width > std::numeric_limits<int>::max() || props.Dimensions.Height > std::numeric_limits<int>::max())
        {
            TerminateGLFW();
            return Result<GLFWwindow*, std::string>::Err("Window width and height must be less than or equal to " +
                                                         std::to_string(std::numeric_limits<int>::max()));
        }

        GLFWwindow* window =
            glfwCreateWindow(static_cast<std::int32_t>(props.Dimensions.Width), static_cast<std::int32_t>(props.Dimensions.Height),
                             props.Title.c_str(), nullptr, nullptr);
        if (window == nullptr)
        {
            TerminateGLFW();
            return Result<GLFWwindow*, std::string>::Err("Failed to create GLFW window");
        }

        s_WindowCount++;
        return Result<GLFWwindow*, std::string>::Ok(window);
    }

    Result<GLFWwindow*, std::string> GLFWWindowHelper::CreateNonAPIWindow(const WindowProps &props)
    {
        if (!InitGLFW())
        {
            return Result<GLFWwindow*, std::string>::Err("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // Create the window
        if (props.Dimensions.Width > std::numeric_limits<int>::max() || props.Dimensions.Height > std::numeric_limits<int>::max())
        {
            TerminateGLFW();
            return Result<GLFWwindow*, std::string>::Err("Window width and height must be less than or equal to " +
                                                         std::to_string(std::numeric_limits<int>::max()));
        }

        GLFWwindow* window =
            glfwCreateWindow(static_cast<std::int32_t>(props.Dimensions.Width), static_cast<std::int32_t>(props.Dimensions.Height),
                             props.Title.c_str(), nullptr, nullptr);
        if (window == nullptr)
        {
            TerminateGLFW();
            return Result<GLFWwindow*, std::string>::Err("Failed to create GLFW window");
        }

        s_WindowCount++;
        return Result<GLFWwindow*, std::string>::Ok(window);
    }

    void GLFWWindowHelper::DestroyWindow(OwnedPtr<GLFWwindow*> window)
    {
        glfwDestroyWindow(window.Get());
        s_WindowCount--;

        if (s_WindowCount == 0)
        {
            TerminateGLFW();
        }
    }

    static void GLFWErrorCallback(int error, const char* description)
    {
        ASTRELIS_CORE_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    bool GLFWWindowHelper::InitGLFW()
    {
        if (!s_GLFWInitialized)
        {
            glfwSetErrorCallback(GLFWErrorCallback);
            if (glfwInit() == GLFW_FALSE)
            {
                return false;
            }

            s_GLFWInitialized = true;
        }

        return true;
    }

    void GLFWWindowHelper::TerminateGLFW()
    {
        glfwTerminate();
        s_GLFWInitialized = false;
    }

    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
    void GLFWWindowHelper::SetEventCallbacks(RawRef<GLFWwindow*> window, BaseWindowData& data)
    {
        glfwSetWindowUserPointer(window, &data);

        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            ViewportResizedEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            WindowClosedEvent event;
            data.EventCallback(event);
        });

        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action,
                                      [[maybe_unused]] int mods) {
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            // NOLINTNEXTLINE(readability-simplify-boolean-expr)
            ASTRELIS_CORE_ASSERT(action == GLFW_PRESS || action == GLFW_RELEASE || action == GLFW_REPEAT,
                               "Invalid action type");
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

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
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            MouseMovedEvent event(static_cast<float>(xpos), static_cast<float>(ypos));
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            // NOLINTNEXTLINE(readability-simplify-boolean-expr)
            ASTRELIS_CORE_ASSERT(action == GLFW_PRESS || action == GLFW_RELEASE, "Invalid action type");
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

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
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            MouseScrolledEvent event(static_cast<float>(xoffset), static_cast<float>(yoffset));
            data.EventCallback(event);
        });

        glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused) {
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

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
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

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
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

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
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            WindowRefreshEvent event;
            data.EventCallback(event);
        });

        glfwSetWindowPosCallback(window, [](GLFWwindow* window, int xpos, int ypos) {
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            WindowMovedEvent event(xpos, ypos);
            data.EventCallback(event);
        });

        glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            WindowResizedEvent event(width, height);
            data.Dimensions.Width  = width;
            data.Dimensions.Height = height;
            data.EventCallback(event);
        });

        glfwSetWindowContentScaleCallback(window, [](GLFWwindow* window, float xscale, float yscale) {
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            WindowScaleEvent event(xscale, yscale);
            data.EventCallback(event);
        });

        glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");
            WindowClosedEvent event;
            data.EventCallback(event);
        });

        glfwSetCursorEnterCallback(window, [](GLFWwindow* window, int entered) {
            auto& data = GLFWWindowHelper::GetUserData<BaseWindowData>(window);
            ASTRELIS_CORE_ASSERT(data.EventCallback, "EventCallback is not set");

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

    std::size_t GLFWWindowHelper::s_WindowCount = 0;
    bool GLFWWindowHelper::s_GLFWInitialized    = false;
} // namespace Astrelis
