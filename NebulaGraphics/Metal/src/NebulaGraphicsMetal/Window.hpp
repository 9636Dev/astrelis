#pragma once


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

#include "NebulaGraphicsCore/Window.hpp"
#include "NebulaGraphicsCore/RenderPass.hpp"

#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>

namespace Nebula
{
    struct MetalWindowData
    {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;

        Window::EventCallbackFunction EventCallback;

        MetalWindowData(std::string title, unsigned int width, unsigned int height, bool vsync) :
            Title(std::move(title)),
            Width(width),
            Height(height),
            VSync(vsync),
            EventCallback([](Event&) {})
        {
        }
    };

    struct MetalRenderPass : public RenderPass
    {
    };

    class MetalWindow : public Window
    {
    public:
        explicit MetalWindow(GLFWwindow* window, MetalWindowData data, MetalContext& ctx);
        ~MetalWindow() override;
        MetalWindow(const MetalWindow&)            = delete;
        MetalWindow& operator=(const MetalWindow&) = delete;
        MetalWindow(MetalWindow&&)                 = delete;
        MetalWindow& operator=(MetalWindow&&)      = delete;

        void SwapBuffers() override;
        void PollEvents() override;
        [[nodiscard]] bool ShouldClose() const override;

        void SetEventCallback(const EventCallbackFunction& callback) override { m_Data.EventCallback = callback; }

        void SetVSync(bool enabled) override;

        [[nodiscard]] bool IsVSync() const override { return m_Data.VSync; }

        [[nodiscard]] unsigned int GetWidth() const override { return m_Data.Width; }

        [[nodiscard]] unsigned int GetHeight() const override { return m_Data.Height; }

        [[nodiscard]] NSWindow* GetNativeWindow() const { return m_NSWindow; }
    private:
        MetalWindowData m_Data;
        id<MTLDevice> m_MetalDevice;
        GLFWwindow* m_Window;
        NSWindow* m_NSWindow;
        CAMetalLayer* m_CAMetalLayer;
    };
} // namespace Nebula


