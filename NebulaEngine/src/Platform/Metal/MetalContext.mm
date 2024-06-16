#import "MetalContext.hpp"

#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

namespace Nebula
{
    class MetalRenderContext::Impl
    {
    public:
        explicit Impl(GLFWwindow* window) : m_Window(window), m_NSWindow(glfwGetCocoaWindow(window)), m_Device(MTLCreateSystemDefaultDevice()), m_MetalLayer([CAMetalLayer layer])
        {
            m_MetalLayer.device = m_Device;
            m_MetalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
            m_MetalLayer.framebufferOnly = YES;
            m_MetalLayer.contentsScale = [m_NSWindow backingScaleFactor];
            [m_NSWindow.contentView setLayer:m_MetalLayer];
            [m_NSWindow.contentView setWantsLayer:YES];
        }

        ~Impl()
        {
            // Just some dummy code
            (void)glfwGetCocoaWindow(m_Window);
        }
        Impl(const Impl&) = delete;
        Impl& operator=(const Impl&) = delete;
        Impl(Impl&&) = delete;
        Impl& operator=(Impl&&) = delete;

        void Init()
        {
        }

        void SwapBuffers()
        {
        }

        GLFWwindow* m_Window;
        NSWindow* m_NSWindow;
        id<MTLDevice> m_Device;
        CAMetalLayer* m_MetalLayer;
    };

    MetalRenderContext::MetalRenderContext(GLFWwindow* window)
        : m_Impl(new Impl(window))
    {
    }

    MetalRenderContext::~MetalRenderContext()
    {
        delete m_Impl;
    }

    void MetalRenderContext::Init()
    {
        m_Impl->Init();
    }

    void MetalRenderContext::SwapBuffers()
    {
        m_Impl->SwapBuffers();
    }

    Ptr<MetalRenderContext> MetalRenderContext::Create(void* window)
    {
        GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window);
        return MakePtr<MetalRenderContext>(glfwWindow);
    }
}  // namespace Nebula
