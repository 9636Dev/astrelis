#include "Window.hpp"

#include "Core.hpp"
#include "NebulaCore/Log.hpp"
#include "NebulaGraphicsCore/Event/KeyEvent.hpp"
#include "NebulaGraphicsCore/Event/MouseEvent.hpp"
#include "NebulaGraphicsCore/Event/WindowEvent.hpp"
#include "NebulaGraphicsCore/Window.hpp"

#include <GLFW/glfw3.h>

namespace Nebula
{
    inline static void SetupGLFWCallbacks(GLFWwindow* window)
    {

        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int newWidth, int newHeight) {
            auto* data = static_cast<GLFWWindowData*>(glfwGetWindowUserPointer(window));
            data->FrameBufferSizeCallback(newWidth, newHeight);
        });

        glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int newWidth, int newHeight) {
            auto* data   = static_cast<GLFWWindowData*>(glfwGetWindowUserPointer(window));
            data->Width  = newWidth;
            data->Height = newHeight;

            WindowResizeEvent event(newWidth, newHeight);
            data->EventCallback(event);
        });

        glfwSetWindowPosCallback(window, [](GLFWwindow* window, int newPosX, int newPosY) {
            auto* data = static_cast<GLFWWindowData*>(glfwGetWindowUserPointer(window));
            WindowMoveEvent event(newPosX, newPosY);
            data->EventCallback(event);
        });

        glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused) {
            auto* data = static_cast<GLFWWindowData*>(glfwGetWindowUserPointer(window));
            if (focused == GLFW_TRUE)
            {
                WindowFocusEvent event;
                data->EventCallback(event);
            }
            else
            {
                WindowLostFocusEvent event;
                data->EventCallback(event);
            }
        });

        glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
            auto* data = static_cast<GLFWWindowData*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            data->EventCallback(event);
        });

        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action,
                                      [[maybe_unused]] int mods) {
            // Check if ImGui wants to capture this event
            auto* data = static_cast<GLFWWindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS: {
                KeyPressedEvent event(static_cast<KeyCode>(key), false);
                data->EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyPressedEvent event(static_cast<KeyCode>(key), true);
                data->EventCallback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyReleasedEvent event(static_cast<KeyCode>(key));
                data->EventCallback(event);
                break;
            }
            default:
                break;
            }
        });

        glfwSetCursorEnterCallback(window, [](GLFWwindow* window, int entered) {
            auto* data = static_cast<GLFWWindowData*>(glfwGetWindowUserPointer(window));
            if (entered == GLFW_TRUE)
            {
                MouseEnteredEvent event;
                data->EventCallback(event);
            }
            else
            {
                MouseLeftEvent event;
                data->EventCallback(event);
            }
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, [[maybe_unused]] int mods) {
            // Check if ImGui wants to capture this event
            auto* data = static_cast<GLFWWindowData*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(static_cast<MouseCode>(button));
                data->EventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
                data->EventCallback(event);
                break;
            }
            default:
                break;
            }
        });

        glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
            // Check if ImGui wants to capture this event
            auto* data = static_cast<GLFWWindowData*>(glfwGetWindowUserPointer(window));
            MouseScrollEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            data->EventCallback(event);
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
            auto* data = static_cast<GLFWWindowData*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
            data->EventCallback(event);
        });
    }

    GLFWWindow::GLFWWindow(GLFWwindow* window, GLFWWindowData data) : m_Window(window), m_Data(std::move(data))
    {
        // Shouldn't fail now, so we can do it in the constructor

        // Set the user pointer to this class, so we can access it in the callback
        glfwSetWindowUserPointer(m_Window, &m_Data);

        // Setup the callbacks
        SetupGLFWCallbacks(m_Window);
    }

    GLFWWindow::~GLFWWindow()
    {
        glfwSetWindowUserPointer(m_Window, nullptr);
        glfwDestroyWindow(m_Window);
        glfwTerminate(); // TODO(9636D): This should be called only when the last
                         // window is destroyed
    }

    void GLFWWindow::SwapBuffers() { glfwSwapBuffers(m_Window); }

    void GLFWWindow::PollEvents() { glfwPollEvents(); }

    bool GLFWWindow::ShouldClose() const { return static_cast<bool>(glfwWindowShouldClose(m_Window)); }

    void GLFWWindow::SetVSync(bool enabled)
    {
        MakeContextCurrent();
        glfwSwapInterval(enabled ? 1 : 0);
        m_Data.VSync = enabled;
    }
} // namespace Nebula
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern "C"
{
    NEBULA_GRAPHICS_OPENGL_API Nebula::WindowCreationResult
        nebulaGraphicsOpenGLCreateGLFWWindow(Nebula::WindowProps<Nebula::OpenGLContext>& props)
    {
        static bool glfwInitialized = false;
        if (!glfwInitialized)
        {
            glfwSetErrorCallback([](int error, const char* description) {
                NEB_CORE_LOG_ERROR("GLFW Error ({0}): {1}", error, description);
            });

            if (glfwInit() != GLFW_TRUE)
            {
                return {nullptr, Nebula::WindowCreationResult::ErrorType::ContextCreationFailed};
            }
            glfwInitialized = true;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, props.Context.MajorVersion);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, props.Context.MinorVersion);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE,
                       GLFW_OPENGL_CORE_PROFILE); // TODO(9636D): Make this configurable,
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on MacOS
        // We need to check for overflow here, because GLFW uses signed integers
        if (props.Width > static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max()) ||
            props.Height > static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max()))
        {
            return {nullptr, Nebula::WindowCreationResult::ErrorType::InvalidArguments};
        }

        // TODO(9636D): Setup glfw error callback, so we can check if it is the
        // version of OpenGL that is not supported or if the window creation failed

        auto* glfwWindow =
            glfwCreateWindow(static_cast<std::int32_t>(props.Width), static_cast<std::int32_t>(props.Height),
                             props.Title.c_str(), nullptr, nullptr);
        if (glfwWindow == nullptr)
        {
            return {nullptr, Nebula::WindowCreationResult::ErrorType::WindowCreationFailed};
        }
        Nebula::GLFWWindowData data(props.Title, props.Width, props.Height, false);
        return {std::make_shared<Nebula::GLFWWindow>(glfwWindow, std::move(data)), Nebula::WindowCreationResult::ErrorType::None};
    }
}
#pragma clang diagnostic pop
