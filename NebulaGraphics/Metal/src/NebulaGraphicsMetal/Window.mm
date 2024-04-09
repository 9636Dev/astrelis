#include "Window.hpp"

#include "Core.hpp"
#include "NebulaCore/Log.hpp"
#include "NebulaGraphicsCore/Event/KeyEvent.hpp"
#include "NebulaGraphicsCore/Event/MouseEvent.hpp"
#include "NebulaGraphicsCore/Event/WindowEvent.hpp"
#include "NebulaGraphicsCore/Window.hpp"

namespace Nebula
{
    inline static void SetupGLFWCallbacks(GLFWwindow* window)
    {
        glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int newWidth, int newHeight) {
            auto* data   = static_cast<MetalWindowData*>(glfwGetWindowUserPointer(window));
            data->Width  = newWidth;
            data->Height = newHeight;

            WindowResizeEvent event(newWidth, newHeight);
            data->EventCallback(event);
        });

        glfwSetWindowPosCallback(window, [](GLFWwindow* window, int newPosX, int newPosY) {
            auto* data = static_cast<MetalWindowData*>(glfwGetWindowUserPointer(window));
            WindowMoveEvent event(newPosX, newPosY);
            data->EventCallback(event);
        });

        glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused) {
            auto* data = static_cast<MetalWindowData*>(glfwGetWindowUserPointer(window));
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
            auto* data = static_cast<MetalWindowData*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            data->EventCallback(event);
        });

        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action,
                                      [[maybe_unused]] int mods) {
            // Check if ImGui wants to capture this event
            auto* data = static_cast<MetalWindowData*>(glfwGetWindowUserPointer(window));

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
            auto* data = static_cast<MetalWindowData*>(glfwGetWindowUserPointer(window));
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
            auto* data = static_cast<MetalWindowData*>(glfwGetWindowUserPointer(window));

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
            auto* data = static_cast<MetalWindowData*>(glfwGetWindowUserPointer(window));
            MouseScrollEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            data->EventCallback(event);
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
            auto* data = static_cast<MetalWindowData*>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
            data->EventCallback(event);
        });
    }

    MetalWindow::MetalWindow(GLFWwindow* window, MetalWindowData data, [[maybe_unused]] MetalContext& ctx) :
        m_MetalDevice(MTL::CreateSystemDefaultDevice()),
        m_Window(window),
        m_Data(std::move(data)),
        m_NSWindow(glfwGetCocoaWindow(window)), m_CAMetalLayer([CAMetalLayer layer])
    {
        glfwSetWindowUserPointer(window, &m_Data);

        m_CAMetalLayer.device             = (__bridge id<MTLDevice>)m_MetalDevice;
        m_CAMetalLayer.pixelFormat        = MTLPixelFormatBGRA8Unorm;
        m_NSWindow.contentView.layer      = m_CAMetalLayer;
        m_NSWindow.contentView.wantsLayer = YES;

        SetupGLFWCallbacks(window);
    }

    MetalWindow::~MetalWindow()
    {
        glfwSetWindowUserPointer(m_Window, nullptr);
        glfwDestroyWindow(m_Window);
        m_MetalDevice->release();
        glfwTerminate();
    }

    void MetalWindow::SwapBuffers() {}

    void MetalWindow::PollEvents() { glfwPollEvents(); }

    bool MetalWindow::ShouldClose() const { return static_cast<bool>(glfwWindowShouldClose(m_Window)); }

    void MetalWindow::SetVSync(bool enabled)
    {
        glfwSwapInterval(enabled ? 1 : 0);
        m_Data.VSync = enabled;
    }

} // namespace Nebula

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern "C"
{
    NEBULA_GRAPHICS_METAL_API Nebula::WindowCreationResult
        /* NOLINT(google-objc-function-naming) */ nebulaGraphicsMetalCreateMetalWindow(Nebula::WindowProps<Nebula::MetalContext>& props)
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

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
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
        Nebula::MetalWindowData data(props.Title, props.Width, props.Height, false);
        return {std::make_shared<Nebula::MetalWindow>(glfwWindow, std::move(data), props.Context),
                Nebula::WindowCreationResult::ErrorType::None};
    }
}
#pragma clang diagnostic pop
