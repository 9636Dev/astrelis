#pragma once

#include "NebulaEngine/Core/Pointer.hpp"

#include "CommandBuffer.hpp"
#include "CommandPool.hpp"
#include "GraphicsPipeline.hpp"
#include "ImageViews.hpp"
#include "RenderPass.hpp"

namespace Nebula
{
    struct Renderer2DStorage
    {
        RefPtr<CommandPool> m_CommandPool           = nullptr;
        RefPtr<CommandBuffer> m_CommandBuffer       = nullptr;
        RefPtr<ImageViews> m_ImageViews             = nullptr;
        RefPtr<RenderPass> m_RenderPass             = nullptr;
        RefPtr<SwapChainFrameBuffers> m_FrameBuffers = nullptr;
        RefPtr<GraphicsPipeline> m_GraphicsPipeline = nullptr;
    };
} // namespace Nebula
