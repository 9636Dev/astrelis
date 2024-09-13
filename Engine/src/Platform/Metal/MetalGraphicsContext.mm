#include "MetalGraphicsContext.hpp"
#include "Astrelis/Core/Base.hpp"

#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#import <objc/objc.h>
#import <objc/runtime.h>

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Astrelis
{
    class MetalGraphicsContext::Impl
    {
    public:
        explicit Impl(RawRef<GLFWwindow*> window) : m_Window(std::move(window)) {}

        ~Impl() = default;

        Result<EmptyType, std::string> Init()
        {
            m_NSWindow   = glfwGetCocoaWindow(m_Window);
            m_MetalLayer = [CAMetalLayer layer];

            m_Device = MTLCreateSystemDefaultDevice();
            if (m_Device == nil)
            {
                return "Failed to create Metal device";
            }

            m_Library = nil;

            m_Initialized = true;
            return EmptyType{};
        }

        void Shutdown() {}

        bool IsInitialized() const { return m_Initialized; }

        void BeginFrame() {}

        void EndFrame() {}

        bool SkipFrame() { return false; }

        bool IsVSync() const { return true; }

        void SetVSync(bool enabled) { ASTRELIS_UNUSED(enabled); }
    private:
        bool m_Initialized = false;

        RawRef<GLFWwindow*> m_Window;
        NSWindow* m_NSWindow       = nil;
        CAMetalLayer* m_MetalLayer = nil;

        id<MTLDevice> m_Device   = nil;
        id<MTLLibrary> m_Library = nil;
    };

    MetalGraphicsContext::MetalGraphicsContext(RawRef<GLFWwindow*> window) : m_Impl(new Impl(std::move(window))) {}

    MetalGraphicsContext::~MetalGraphicsContext() { delete m_Impl; }

    Result<EmptyType, std::string> MetalGraphicsContext::Init() { return m_Impl->Init(); }

    void MetalGraphicsContext::Shutdown() {}

    bool MetalGraphicsContext::IsInitialized() const { return m_Impl->IsInitialized(); }

    void MetalGraphicsContext::BeginFrame() { m_Impl->BeginFrame(); }

    void MetalGraphicsContext::EndFrame() { m_Impl->EndFrame(); }

    bool MetalGraphicsContext::SkipFrame() { return m_Impl->SkipFrame(); }

    bool MetalGraphicsContext::IsVSync() const { return m_Impl->IsVSync(); }

    void MetalGraphicsContext::SetVSync(bool enabled) { m_Impl->SetVSync(enabled); }

    std::uint32_t MetalGraphicsContext::GetCurrentFrameIndex() const { return 0; }

    std::uint32_t MetalGraphicsContext::GetImageIndex() const { return 0; }

    RefPtr<MetalGraphicsContext> Create(void* window);
} // namespace Astrelis
