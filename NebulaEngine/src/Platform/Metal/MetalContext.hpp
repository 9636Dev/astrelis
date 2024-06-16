#pragma once

#include "NebulaEngine/Renderer/RenderContext.hpp"

struct GLFWwindow;

namespace Nebula
{
    class MetalRenderContext : public RenderContext
    {
    public:
        explicit MetalRenderContext(GLFWwindow* window);
        ~MetalRenderContext() override;
        MetalRenderContext(const MetalRenderContext&)            = delete;
        MetalRenderContext& operator=(const MetalRenderContext&) = delete;
        MetalRenderContext(MetalRenderContext&&)                 = delete;
        MetalRenderContext& operator=(MetalRenderContext&&)      = delete;

        void Init() override;
        void SwapBuffers() override;

        static Ptr<MetalRenderContext> Create(void* window);
    private:
        class Impl;
        Impl* m_Impl;
    };

} // namespace Nebula
